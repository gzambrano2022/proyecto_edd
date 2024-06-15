#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>
#include <vector>
#include <fstream>

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
        HuffmanCoder(){}

        std::string codificar(const std::string& texto){
            contar_frecuencias(texto);
            Nodo* arbol = construir_arbol();
            codigos.clear();
            decodigos.clear();
            generar_codigos(arbol, "");

            std::string codigo;
            for(char c : texto){
                codigo += codigos[c];
            }

            liberar_arbol(arbol);
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
            std::cout << "Frecuencias de los caracteres:" << std::endl;
            for(auto& pair : frecuencias){
                std::cout << "caracter: " << pair.first << " frecuencia: " << pair.second << std::endl;
            }
        }
        double obtener_peso_archivo(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            std::cerr << "No se puede abrir el archivo: " << filename << std::endl;
            return 0.0;
        }
        std::streamsize size = file.tellg();
        file.close();
        return size / 1000.0; // tamaño en kilobytes
    }

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

int main(){
    std::string texto = "comere";
    HuffmanCoder huffman;

    std::string codigo = huffman.codificar(texto);
    huffman.mostrar_frecuencias();

    std::string texto_decodificado = huffman.decodificar(codigo);
    std::cout << "Texto original: " << texto << std::endl;
    std::cout << "Texto codificado: " << codigo << std::endl;
    std::cout << "Texto decodificado: " << texto_decodificado << std::endl;

    huffman.escribir_a_archivo("texto_original.txt", texto);
    huffman.escribir_a_archivo("texto_codificado.txt", codigo);
    huffman.escribir_a_archivo("texto_decodificado.txt", texto_decodificado);

    double peso_original = huffman.obtener_peso_archivo("texto_original.txt");
    double peso_codificado = huffman.obtener_peso_archivo("texto_codificado.txt");
    double peso_decodificado = huffman.obtener_peso_archivo("texto_decodificado.txt");

    std::cout << "Peso del texto original (en kilobytes): " << peso_original << " KB" << std::endl;
    std::cout << "Peso del texto codificado (en kilobytes): " << peso_codificado << " KB" << std::endl;
    std::cout << "Peso del texto decodificado (en kilobytes): " << peso_decodificado << " KB" << std::endl;
}
