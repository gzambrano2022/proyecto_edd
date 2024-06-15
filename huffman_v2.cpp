#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <iterator>
#include <chrono>

//código sacados de https://github.com/Fatima-Mujahid/huffman-coding/blob/main/HuffmanCoding.cpp 

struct Nodo{
    char caracter;
    int frecuencia;
    Nodo* izq;
    Nodo* der;

    Nodo(char c, int f) : caracter(c), frecuencia(f), izq(nullptr), der(nullptr) {}
};

struct Comparador{
    bool operator()(Nodo* a, Nodo* b){
        return a->frecuencia > b->frecuencia;
    }
};

class HuffmanCoder{
    private:
        std::unordered_map<char, int> frecuencias;
        std::unordered_map<char, std::string> codigos;
        std::unordered_map<std::string, char> decodigos;
        Nodo* arbol;

        void contar_frecuencias(const std::string& texto){
            frecuencias.clear(); // Limpiar frecuencias anteriores
            for(char c : texto){
                frecuencias[c]++;
            }
        }

        Nodo* construir_arbol(){
            std::priority_queue<Nodo*, std::vector<Nodo*>, Comparador> min_heap;

            // Crear un nodo para cada carácter y los añade a la cola de prioridad.
            for(auto& pair: frecuencias){ // auto es una palabra que permite al compilador deducir automáticamente el tipo de una variable
                min_heap.push(new Nodo(pair.first, pair.second)); // pair.first es el carácter y pair.second es la frecuencia del carácter.
            }

            while(min_heap.size() > 1){
                Nodo* nodo1 = min_heap.top();
                min_heap.pop();
                Nodo* nodo2 = min_heap.top();
                min_heap.pop();

                Nodo* combinado = new Nodo('\0', nodo1->frecuencia + nodo2->frecuencia);
                combinado->izq = nodo1;
                combinado->der = nodo2;

                min_heap.push(combinado);
            }

            return min_heap.top();
        }

        void generar_codigos(Nodo* nodo, const std::string& codigo_actual) {
            if (!nodo){
                return;
            }

            if (nodo->caracter != '\0') {
                codigos[nodo->caracter] = codigo_actual;
                decodigos[codigo_actual] = nodo->caracter;
            }

            generar_codigos(nodo->izq, codigo_actual + "0");
            generar_codigos(nodo->der, codigo_actual + "1");
        }

        void liberar_arbol(Nodo* nodo){
            if(!nodo){
                return;
            }
            liberar_arbol(nodo->izq);
            liberar_arbol(nodo->der);
            delete nodo;
        }

    public:
        HuffmanCoder(): arbol(nullptr){}

        ~HuffmanCoder(){
            if (arbol){
                liberar_arbol(arbol);
            }
        }

        std::string codificar(const std::string& texto){
            contar_frecuencias(texto);
            if(arbol){
                liberar_arbol(arbol);
            }
            arbol = construir_arbol();
            codigos.clear();
            decodigos.clear();
            generar_codigos(arbol, "");

            std::string codigo;
            for(char c : texto){
                codigo += codigos[c];
            }

            return codigo;
        }

        std::string decodificar(const std::string& codigo){
            std::string codigo_actual;
            std::string texto_decodificado;

            for(char bit: codigo){
                codigo_actual += bit;
                if(decodigos.find(codigo_actual) != decodigos.end()){
                    texto_decodificado += decodigos[codigo_actual];
                    codigo_actual.clear();
                }
            }
            return texto_decodificado;
        }

        void mostrar_frecuencias(){
            for(auto& pair : frecuencias){
                std::cout << "caracter: " << pair.first << "(" << pair.second << ")" <<std::endl;
            }
        }
};

int main(){
    std::ifstream archivo("test.txt");
    if(!archivo.is_open()){
        std::cout << "No se pudo abrir el archivo." << std::endl;
        return 1;
    }

    std::string texto((std::istreambuf_iterator<char>(archivo)), std::istreambuf_iterator<char>());
    archivo.close();


    HuffmanCoder huffman;

    auto inicio_cod = std::chrono::high_resolution_clock::now();
    std::string codigo = huffman.codificar(texto);
    auto fin_cod = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion_cod = fin_cod - inicio_cod;

    //huffman.mostrar_frecuencias();

    auto inicio_decod = std::chrono::high_resolution_clock::now();
    std::string texto_decodificado = huffman.decodificar(codigo);
    auto fin_decod = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion_decod = fin_decod - inicio_decod;


    //std::cout << "Texto original: " << texto << std::endl;
    //std::cout << "Texto codificado: " << codigo << std::endl;
    //std::cout << "Texto decodificado: " << texto_decodificado << std::endl;

    std::cout << "Tiempo de codificacion: " << duracion_cod.count() << std::endl;
    std::cout << "Tiempo de decodificacion: " << duracion_decod.count() << std::endl;
}
