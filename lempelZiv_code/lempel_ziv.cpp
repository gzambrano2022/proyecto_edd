#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <sstream>

// Estructura de Nodo para árbol de sufijos compacto
struct SuffixNode {
    int start;  // Índice de inicio del sufijo
    int end;    // Índice de fin del sufijo
    int depth;  // Profundidad del nodo en el árbol
    SuffixNode* parent;
    std::vector<SuffixNode*> children;

    SuffixNode(int start, int end, int depth, SuffixNode* parent)
        : start(start), end(end), depth(depth), parent(parent) {}
};

// Clase que implementa el árbol de sufijos compacto
class SuffixTree {
private:
    SuffixNode* root;  // Nodo raíz del árbol
    std::string input; // Texto original

    // Construir el árbol de sufijos compacto
    void buildSuffixTree(const std::string& input) {
        root = new SuffixNode(-1, -1, 0, nullptr);
        int n = input.size();
        std::vector<SuffixNode*> nodes(n);

        // Insertar cada sufijo en el árbol
        for (int i = 0; i < n; ++i) {
            insertSuffix(input, i, root, nodes);
        }
    }

    // Insertar un sufijo en el árbol
    void insertSuffix(const std::string& input, int start, SuffixNode* current, std::vector<SuffixNode*>& nodes) {
        int n = input.size();
        SuffixNode* parent = current;

        while (true) {
            SuffixNode* child = nullptr;

            // Buscar el hijo correspondiente
            for (auto* ch : parent->children) {
                if (input[ch->start] == input[start]) {
                    child = ch;
                    break;
                }
            }

            if (child == nullptr) {
                // Si no hay coincidencia, crear un nuevo nodo
                SuffixNode* leaf = new SuffixNode(start, n - 1, n - start, parent);
                nodes.push_back(leaf);
                parent->children.push_back(leaf);
                return;
            }

            // Buscar la longitud de la coincidencia
            int len = std::min(child->end - child->start + 1, n - start);
            int i = child->start;

            while (i <= child->end && input[i] == input[start]) {
                ++i;
                ++start;
            }

            if (i <= child->end) {
                // Dividir el nodo existente
                SuffixNode* splitNode = new SuffixNode(child->start, i - 1, child->depth, parent);
                nodes.push_back(splitNode);
                parent->children.push_back(splitNode);
                child->start = i;
                child->parent = splitNode;
                splitNode->children.push_back(child);
                continue;
            } else {
                // Avanzar al siguiente nodo
                parent = child;
                start -= len;
            }
        }
    }

    // Método privado para buscar la coincidencia más larga en el árbol de sufijos
    std::pair<int, int> searchLongestMatch(const std::string& s, int startPos) {
        SuffixNode* node = root;
        int length = 0;
        int pos = -1;
        int n = s.size();
        int originalStartPos = startPos;

        while (startPos < n) {
            SuffixNode* child = nullptr;

            // Buscar el hijo correspondiente
            for (auto* ch : node->children) {
                if (input[ch->start] == s[startPos]) {
                    child = ch;
                    break;
                }
            }

            if (child == nullptr) {
                break;
            }

            // Avanzar al siguiente nodo
            node = child;
            int matchStart = startPos;
            int nodeStart = child->start;

            // Verificar la longitud de la coincidencia
            while (nodeStart <= child->end && matchStart < n && input[nodeStart] == s[matchStart]) {
                if (nodeStart != -1 && nodeStart < originalStartPos) {
                    length++;
                    pos = nodeStart - length + 1;
                }
                nodeStart++;
                matchStart++;
            }

            startPos = matchStart;

            if (nodeStart <= child->end) {
                break;
            }
        }

        return {pos, length};
    }

public:
    // Constructor
    SuffixTree(const std::string& input) : input(input) {
        buildSuffixTree(input);
    }

    // Método para comprimir usando el algoritmo Lempel-Ziv
    std::vector<std::pair<std::string, int>> compress(const std::string& input) {
        std::vector<std::pair<std::string, int>> output;
        int i = 0;

        while (i < input.size()) {
            auto match = searchLongestMatch(input, i);

            if (match.first == -1 || match.second == 0) {
                output.push_back({std::string(1, input[i]), 0});
                i++;
            } else {
                output.push_back({std::to_string(match.first), match.second});
                i += match.second;
            }
        }

        return output;
    }

    // Método para descomprimir
    std::string decompress(const std::vector<std::pair<std::string, int>>& compressed) {
        std::string decompressed;

        for (const auto& p : compressed) {
            if (p.second == 0) {
                decompressed += p.first;
            } else {
                int pos = std::stoi(p.first);
                int length = p.second;
                for (int i = 0; i < length; ++i) {
                    decompressed += decompressed[pos + i];
                }
            }
        }

        return decompressed;
    }

    // Método para obtener el tamaño del archivo en KB
    double obtener_peso_archivo(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);

        if (!file.is_open()) {
            std::cerr << "No se puede abrir el archivo: " << filename << std::endl;
            return 0.0;
        }

        std::streamsize size = file.tellg();
        file.close();
        return size / 1000.0;
    }

    // Método para escribir a archivo
    void escribir_a_archivo(const std::string& filename, const std::string& texto) {
        std::ofstream file(filename);

        if (file.is_open()) {
            file << texto;
            file.close();
        } else {
            std::cerr << "No se puede abrir el archivo para escribir: " << filename << std::endl;
        }
    }
};

int main() {
    std::ifstream archivo("test.txt");
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo." << std::endl;
        return 1;
    }

    std::string texto((std::istreambuf_iterator<char>(archivo)), std::istreambuf_iterator<char>());
    archivo.close();

    SuffixTree suffixTree(texto);

    auto inicio_com = std::chrono::high_resolution_clock::now();
    auto compressed = suffixTree.compress(texto);
    auto fin_com = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion_com = fin_com - inicio_com;

    std::stringstream texto_comprimido_ss;
    std::string texto_comprimido;

    for (const auto& p : compressed) {
        texto_comprimido_ss << "(" << p.first << "," << p.second << ") ";
    }

    texto_comprimido = texto_comprimido_ss.str();

    auto inicio_decom = std::chrono::high_resolution_clock::now();
    std::string decompressed = suffixTree.decompress(compressed);
    auto fin_decom = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion_decom = fin_decom - inicio_decom;

    suffixTree.escribir_a_archivo("texto_comprimido.txt", texto_comprimido);
    suffixTree.escribir_a_archivo("texto_descomprimido.txt", decompressed);

    double peso_original = suffixTree.obtener_peso_archivo("test.txt");
    double peso_comprimido = suffixTree.obtener_peso_archivo("texto_comprimido.txt");
    double peso_descomprimido = suffixTree.obtener_peso_archivo("texto_descomprimido.txt");

    std::cout << "Tamanos: " << std::endl;
    std::cout << "Peso del texto original (en kilobytes): " << peso_original << " KB" << std::endl;
    std::cout << "Peso del texto comprimido (en kilobytes): " << peso_comprimido << " KB" << std::endl;
    std::cout << "Peso del texto descomprimido (en kilobytes): " << peso_descomprimido << " KB" << std::endl << std::endl;

    std::cout << "Tiempo: " << std::endl;
    std::cout << "Tiempo de compresion: " << duracion_com.count() << " segundos" << std::endl;
    std::cout << "Tiempo de decompresion: " << duracion_decom.count() << " segundos" << std::endl;

    return 0;
}
