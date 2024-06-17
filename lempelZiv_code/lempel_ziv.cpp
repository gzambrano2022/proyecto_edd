#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iterator>
#include <chrono>
#include <sstream>

//Estructura que representa un nodo del Trie
struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    int firstPosition;
    TrieNode() : firstPosition(-1) {}
};

//Clase que implementa el Trie
class Trie {
private:
    TrieNode* root;

    //Busca la coincidencia mas larga empezando desde starPos en el string s (en el Trie)
    std::pair<int, int> searchLongestMatch(const std::string& s, int startPos) {
        TrieNode* node = root; //Inicializa el nodo actual como la raíz del Trie
        int length = 0; 
        int pos = -1; 

        for (int i = startPos; i < s.size(); ++i) {
            char ch = s[i];

            //Si el caracter no esta en los hijos del nodo se rompe el ciclo
            if (node->children.find(ch) == node->children.end()) {
                break;
            }

            //Avanza al siguiente nodo
            node = node->children[ch];

            //Verifica si el nodo tiene posición valida y esta antes del starPos
            if (node->firstPosition != -1 && node->firstPosition < startPos) {
                length = i - startPos + 1;
                pos = node->firstPosition;
            }
        }

        //Retorna el par (Posición, Longitud)
        return {pos, length};
    }

    //Inserta un substring empezando desde startPos (en el Trie)
    void insert(const std::string& s, int startPos) {
        TrieNode* node = root; //Inicializa el nodo actual como la raíz del Trie


        for (int i = startPos; i < s.size(); ++i) {
            char ch = s[i];

            //Si el caracter no esta en los hijos del nodo crea un nuevo nodo y lo agrega como hijo de la clave ch
            if (node->children.find(ch) == node->children.end()) {
                node->children[ch] = new TrieNode();
            }

            //Avanza al siguiente nodo
            node = node->children[ch];

            //Registrada la posición inicial de la secuencia si es que no esta registrada 
            if (node->firstPosition == -1) {
                node->firstPosition = startPos;
            }
        }
    }

public:
    Trie() : root(new TrieNode()) {} //Constructor de la clase

    //Método que comprime empleando el algoritmo Lempel-Ziv
    std::vector<std::pair<std::string, int>> compress(const std::string& input) {
        std::vector<std::pair<std::string, int>> output; //Vector que almacena la salida comprimida
        int i = 0; //Ídice que recorre el string

        //Itera todo el string 
        while (i < input.size()) {
            auto match = searchLongestMatch(input, i);//Busca la coincidencia mas larga a partir de la posición i del string

            //Verifica si no hay coincidencia o la longitud de esta es 0
            if (match.first == -1 || match.second == 0) {
                output.push_back({std::string(1, input[i]), 0}); //Inserta el char con longitud 0 al vector output
                insert(input, i); //Inserta el substring a partir el índice i
                i++;
            } 

            //Si hay coincidencia
            else {
                output.push_back({std::to_string(match.first), match.second}); //Inserta posición y longitud 
                insert(input, i); //Inserta el substring a partir el índice i
                i += match.second;
            }
        }

        //Retorna el vector con la salida comprimida
        return output;
    }

    //Método que descomprime 
    std::string decompress(const std::vector<std::pair<std::string, int>>& compressed) {
        std::string decompressed;//String para el texto descomprimido

        //Itera sobre cada par (string,int)
        for (const auto& p : compressed) {

            //Si el entero es 0 es un char individual
            if (p.second == 0) {
                decompressed += p.first; //Agrega el char al string decompressed
            } 
            //Si es mayor a 0
            else {
                int pos = std::stoi(p.first); //Transforma el primer par (string) a entero
                int length = p.second; //Obtiene la longitud 
                for (int i = 0; i < length; ++i) {
                    decompressed += decompressed[pos + i]; //Agrega cada char al string decompressed
                }
            }
        }

        //Retorna el string del texto descomprimido
        return decompressed;
    }

    //Método para obtener el tamaño del archivo en KB
    double obtener_peso_archivo(const std::string& filename) {

        // Abre el archivo en modo binario y coloca el puntero al final del archivo
        std::ifstream file(filename, std::ios::binary | std::ios::ate);

        // Verifica si se pudo abrir el archivo correctamente
        if (!file.is_open()) {
            std::cerr << "No se puede abrir el archivo: " << filename << std::endl;
            return 0.0;
        }
        
        // Obtiene el tamaño del archivo colocando el puntero al final y usando tellg()
        std::streamsize size = file.tellg();
        file.close();
        return size / 1000.0; // tamaño en kilobytes
    }

    //Método para hacer .txt un texto
    void escribir_a_archivo(const std::string& filename, const std::string& texto) {

        // Abre un archivo de salida con el nombre especificado por 'filename'
        std::ofstream file(filename);

        // Si el archivo se abrió correctamente y escribe el contenido del texto
        if (file.is_open()) {
            file << texto;
            file.close();
        } else {
            std::cerr << "No se puede abrir el archivo para escribir: " << filename << std::endl;
        }
    }
};


int main() {
    // Abrir el archivo
    std::ifstream archivo("test.txt");
    if(!archivo.is_open()){
        std::cout << "No se pudo abrir el archivo." << std::endl;
        return 1;
    }

    //Leer el contenido en un string
    std::string texto((std::istreambuf_iterator<char>(archivo)), std::istreambuf_iterator<char>());
    archivo.close();


    Trie trie; //Instancia de Trie

    //Medir tiempo de compresión
    std::cout << "Texto original: " << texto << std::endl;
    auto inicio_com = std::chrono::high_resolution_clock::now();
    auto compressed = trie.compress(texto);
    auto fin_com = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion_com = fin_com - inicio_com;

    //Instancia de string comprimida
    std::stringstream texto_comprimido_ss;
    std::string texto_comprimido;

    std::cout << "Compressed output: ";
    for (const auto& p : compressed) {
        std::cout << "(" << p.first << "," << p.second << ") ";
        texto_comprimido_ss << "(" << p.first << "," << p.second << ") ";
    }
    std::cout << std::endl;

    texto_comprimido = texto_comprimido_ss.str();

    //Medir tiempo de descompresión
    auto inicio_decom = std::chrono::high_resolution_clock::now();
    std::string decompressed = trie.decompress(compressed);
    auto fin_decom = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion_decom = fin_decom - inicio_decom;

    std::cout << "Decompressed output: " << decompressed;

    //Escribir los textos de compresión y descompresión en un .txt
    trie.escribir_a_archivo("texto_comprimido.txt", texto_comprimido);
    trie.escribir_a_archivo("texto_descomprimido.txt", decompressed);

    //Tamaños de los textos (KB)
    double peso_original = trie.obtener_peso_archivo("test.txt");
    double peso_comprimido = trie.obtener_peso_archivo("texto_comprimido.txt");
    double peso_descomprimido = trie.obtener_peso_archivo("texto_descomprimido.txt");

    //Imprime los tamaños de los textos (KB)
    std::cout << "Tamanos: " << std::endl;
    std::cout << "Peso del texto original (en kilobytes): " << peso_original << " KB" << std::endl;
    std::cout << "Peso del texto comprimido (en kilobytes): " << peso_comprimido << " KB" << std::endl;
    std::cout << "Peso del texto descomprimido (en kilobytes): " << peso_descomprimido << " KB" << std::endl << std::endl;

    //Imprime los tiempos de compresión y Descompresión
    std::cout << "Tiempo: " <<  std::endl;
    std::cout << "Tiempo de compresion: " << duracion_com.count() << std::endl;
    std::cout << "Tiempo de decompresion: " << duracion_decom.count() << std::endl;

    return 0;
}
