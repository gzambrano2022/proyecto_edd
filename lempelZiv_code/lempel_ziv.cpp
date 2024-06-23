/* Algoritmo optimizado, ahora se usa unordered_map, inserción de datos al árbol O(n) (Algoritmo de Ukkonen) y mejor manejo de memoria */
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <string>
#include <unordered_map>

// Estructura de Nodo para árbol de sufijos compacto
struct SuffixNode {
    int start;  // Índice de inicio del sufijo
    int* end;    // Índice de fin del sufijo
    int suffixIndex;  // Índice del sufijo en el texto original
    std::unordered_map<char, SuffixNode*> children;

    SuffixNode(int start, int* end) : start(start), end(end), suffixIndex(-1) {}
};

// Clase que implementa el árbol de sufijos compacto
class SuffixTree {
private:
    SuffixNode* root;  // Nodo raíz del árbol
    std::string input; // Texto original
    int* leafEnd;

    // Construir el árbol de sufijos compacto usando el algoritmo de Ukkonen
    void buildSuffixTree(const std::string& input) {
        this->input = input;
        leafEnd = new int(-1);
        root = new SuffixNode(-1, new int(-1));
        int size = input.size();

        for (int i = 0; i < size; i++) {
            extendSuffixTree(i);
        }
    }

    // Extender el árbol de sufijos con el siguiente sufijo
    void extendSuffixTree(int pos) {
        static SuffixNode* activeNode = root; // Nodo activo
        static int activeEdge = -1; // Borde activo
        static int activeLength = 0; // Longitud activa

        leafEnd = new int(pos);
        int remainingSuffixCount = 0;

        // Extender con el siguiente carácter
        while (remainingSuffixCount <= pos) {
            if (activeLength == 0) {
                activeEdge = pos;
            }

            if (activeNode->children.find(input[activeEdge]) == activeNode->children.end()) {
                // Crear un nuevo nodo hoja
                activeNode->children[input[activeEdge]] = new SuffixNode(pos, leafEnd);

                if (activeNode != root) {
                    activeNode = root;
                }
            } else {
                SuffixNode* next = activeNode->children[input[activeEdge]];
                int edgeLength = *next->end - next->start + 1;

                if (activeLength >= edgeLength) {
                    activeEdge += edgeLength;
                    activeLength -= edgeLength;
                    activeNode = next;
                    continue;
                }

                if (input[next->start + activeLength] == input[pos]) {
                    activeLength++;
                    break;
                }

                int* splitEnd = new int(next->start + activeLength - 1);
                SuffixNode* split = new SuffixNode(next->start, splitEnd);
                activeNode->children[input[activeEdge]] = split;
                split->children[input[pos]] = new SuffixNode(pos, leafEnd);
                next->start += activeLength;
                split->children[input[next->start]] = next;

                if (activeNode != root) {
                    activeNode = root;
                }
            }
            remainingSuffixCount++;
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
            for (auto& [key, value] : node->children) {
                if (input[value->start] == s[startPos]) {
                    child = value;
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
            while (nodeStart <= *child->end && matchStart < n && input[nodeStart] == s[matchStart]) {
                if (nodeStart != -1 && nodeStart < originalStartPos) {
                    length++;
                    pos = nodeStart - length + 1;
                }
                nodeStart++;
                matchStart++;
            }

            startPos = matchStart;

            if (nodeStart <= *child->end) {
                break;
            }
        }

        return {pos, length};
    }

public:
    // Constructor
    SuffixTree(const std::string& input) {
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

    // Método para escribir a archivo de texto
    void escribir_a_archivo(const std::string& filename, const std::string& texto) {
        std::ofstream file(filename);

        if (file.is_open()) {
            file << texto;
            file.close();
        } else {
            std::cerr << "No se puede abrir el archivo para escribir: " << filename << std::endl;
        }
    }

    // Método para escribir el archivo comprimido en formato binario
    void escribir_a_archivo_binario(const std::string& filename, const std::vector<std::pair<std::string, int>>& compressed) {
        std::ofstream file(filename, std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "No se puede abrir el archivo para escribir: " << filename << std::endl;
            return;
        }

        for (const auto& p : compressed) {
            int offset = (p.second == 0) ? 0 : std::stoi(p.first);
            int length = p.second;

            file.write(reinterpret_cast<char*>(&offset), sizeof(offset));
            file.write(reinterpret_cast<char*>(&length), sizeof(length));

            if (length == 0) {
                char literal = p.first[0];
                file.write(&literal, sizeof(literal));
            }
        }

        file.close();
    }

    // Método para leer el archivo comprimido en formato binario
    std::vector<std::pair<std::string, int>> leer_de_archivo_binario(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        std::vector<std::pair<std::string, int>> compressed;

        if (!file.is_open()) {
            std::cerr << "No se puede abrir el archivo para leer: " << filename << std::endl;
            return compressed;
        }

        while (!file.eof()) {
            int offset;
            int length;

            file.read(reinterpret_cast<char*>(&offset), sizeof(offset));
            file.read(reinterpret_cast<char*>(&length), sizeof(length));

            if (file.eof()) break; // Para evitar leer un registro incompleto al final del archivo

            if (length == 0) {
                char literal;
                file.read(&literal, sizeof(literal));
                compressed.push_back({std::string(1, literal), length});
            } else {
                compressed.push_back({std::to_string(offset), length});
            }
        }

        file.close();
        return compressed;
    }
};

int main(int argc, char const* argv[]) {
    // Si se quiere ejecutar con un texto manualmente, comentar este bloque de código y leer el siguiente comentario
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <archivo_de_entrada>" << std::endl;
        return 1;
    }    

    std::string nombre_archivo = argv[1];
    std::ifstream archivo(nombre_archivo);// archivo(test.txt) si se quiere ejecutar manualmente, se pueden escribir textos de prueba en dicho archivo
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

    suffixTree.escribir_a_archivo_binario("texto_comprimido.bin", compressed);

    auto compressed_bin = suffixTree.leer_de_archivo_binario("texto_comprimido.bin");

    auto inicio_decom = std::chrono::high_resolution_clock::now();
    std::string decompressed = suffixTree.decompress(compressed_bin);
    auto fin_decom = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion_decom = fin_decom - inicio_decom;

    suffixTree.escribir_a_archivo("texto_descomprimido.txt", decompressed);

    double peso_original = suffixTree.obtener_peso_archivo(nombre_archivo);
    double peso_comprimido = suffixTree.obtener_peso_archivo("texto_comprimido.bin");
    double peso_descomprimido = suffixTree.obtener_peso_archivo("texto_descomprimido.txt");

    std::cout << "Peso del texto original (en kilobytes): " << peso_original << " KB" << std::endl;
    std::cout << "Peso del texto comprimido (en kilobytes): " << peso_comprimido << " KB" << std::endl;
    std::cout << "Peso del texto descomprimido (en kilobytes): " << peso_descomprimido << " KB" << std::endl << std::endl;

    std::cout << "Tiempo de compresion: " << duracion_com.count() << " segundos" << std::endl;
    std::cout << "Tiempo de decompresion: " << duracion_decom.count() << " segundos" << std::endl;

    return 0;
}

