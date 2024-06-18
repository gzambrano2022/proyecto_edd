#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <iterator>
#include <chrono>

//código sacados de https://github.com/Fatima-Mujahid/huffman-coding/blob/main/HuffmanCoding.cpp 

//Estructura para el nodo de huffman
struct Nodo{
    char caracter; // Char en nodo
    int frecuencia; // Frecuencia del char
    Nodo* izq; // Punterlo al hijo izquierdo
    Nodo* der; // Puntero al hijo derecho

    // Constructor con char y frecuencia inicializada y los punteros en nullptr
    Nodo(char c, int f) : caracter(c), frecuencia(f), izq(nullptr), der(nullptr) {}
};

// Estructura para comparar
struct Comparador{

    //Compara 2 punteros a nodo y retorna true si la frecuencia del nodo "a" es mayor a la del nodo "b"
    bool operator()(Nodo* a, Nodo* b){
        return a->frecuencia > b->frecuencia;
    }
};

// Clase para el codificador y decodificador
class HuffmanCoder{
    private:
        std::unordered_map<char, int> frecuencias; // Almacena las frecuencias de cada caracter
        std::unordered_map<char, std::string> codigos; // Almacena los códigos Huffman de cada caracter
        std::unordered_map<std::string, char> decodigos; // Almacena la correspondencia inversa de los códigos Huffman
        Nodo* arbol; // Puntero a la raiz del arbol

        // Cuenta las frecuencias de cada caracter en el texto
        void contar_frecuencias(const std::string& texto){
            frecuencias.clear(); // Limpiar frecuencias anteriores
            for(char c : texto){
                frecuencias[c]++;
            }
        }

        // Construye el árbol Huffman a partir de las frecuencias
        Nodo* construir_arbol(){

            // Cola de prioridad para almacenar nodos
            std::priority_queue<Nodo*, std::vector<Nodo*>, Comparador> min_heap;

            // Crear un nodo para cada carácter y los añade a la cola de prioridad.
            for(auto& pair: frecuencias){ // auto es una palabra que permite al compilador deducir automáticamente el tipo de una variable
                min_heap.push(new Nodo(pair.first, pair.second)); // pair.first es el carácter y pair.second es la frecuencia del carácter.
            }

            // Combina nodos hasta que quede uno solo en la cola
            while(min_heap.size() > 1){
                //Extrae los nodos con menor frecuencia
                Nodo* nodo1 = min_heap.top(); // Obtiene el nodo con la menor frecuencia
                min_heap.pop(); // Lo remueve de la cola
                Nodo* nodo2 = min_heap.top(); // Obtiene el siguiente nodo con la menor frecuencia
                min_heap.pop(); // Lo remueve de la cola

                // Crea un nuevo nodo combinado de caracter "\0" con los nodos extraidos
                Nodo* combinado = new Nodo('\0', nodo1->frecuencia + nodo2->frecuencia);
                combinado->izq = nodo1; // Establece el primer nodo extraído como hijo izquierdo
                combinado->der = nodo2; // Establece el segundo nodo extraído como hijo derecho

                // Añade el nuevo nodo combinado a la cola de prioridad
                min_heap.push(combinado);
            }

            // Retorna el nodo raiz que es el unico restante
            return min_heap.top();
        }

        void generar_codigos(Nodo* nodo, const std::string& codigo_actual) {

            // Verifica si el nodo es nulo
            if (!nodo){
                return;
            }

            // Verifica si el nodo actual es una hoja 
            if (nodo->caracter != '\0') {
                codigos[nodo->caracter] = codigo_actual; // Asigna el código actual al carácter en el mapa de codigos
                decodigos[codigo_actual] = nodo->caracter; // Asigna el carácter al código actual en el mapa de decodigos
            }

            generar_codigos(nodo->izq, codigo_actual + "0"); // Llama recursivamente a generar_codigos para el hijo izquierdo con código actual + "0"
            generar_codigos(nodo->der, codigo_actual + "1"); // Llama recursivamente a generar_codigos para el hijo derecho con código actual + "1"
        }

        void liberar_arbol(Nodo* nodo){

            // Verifica si el nodo es nulo
            if(!nodo){
                return;
            }
            // Llama recursivamente a liberar_arbol para el hijo izquierdo y derecho del nodo actual
            liberar_arbol(nodo->izq);
            liberar_arbol(nodo->der);

            // Libera la memoria del nodo actual
            delete nodo;
        }

    public:

        // Constructor de la clase 
        HuffmanCoder(): arbol(nullptr){}
        
        // Destructor de la clase
        ~HuffmanCoder(){
            if (arbol){
                liberar_arbol(arbol); // Liberar memoria del árbol
            }
        }

        // Método para codificar un texto
        std::string codificar(const std::string& texto){
            contar_frecuencias(texto);// Calcula las frecuencias de cada caracter en el texto

            // Verifica si el arbol existe
            if(arbol){
                liberar_arbol(arbol); // Libera la memoria del árbol
            }

            arbol = construir_arbol(); // Construye el arbol
            codigos.clear(); // Limpia el mapa de códigos
            decodigos.clear(); // Limpia el mapa de decódigos
            generar_codigos(arbol, ""); // Genera los codigos para cada caracter

            std::string codigo; // Variable para almacenar el texto codificado

            // Recorre cada caracter en el texto y construye el código Huffman
            for(char c : texto){
                codigo += codigos[c]; // Concatena el código del caracter actual al código completo
            }

            return codigo; // Retorna el código en formato Huffman
        }

        std::string decodificar(const std::string& codigo){
            std::string codigo_actual; // Variable para ir almacenando el código
            std::string texto_decodificado; // Variable para almacenar el código final

            // Itera a traves de cada bit en el código
            for(char bit: codigo){

                codigo_actual += bit; // Agrega el bit actual a la variable que va almacenando el código

                // Verifica si el código actual esta en el mapa decodigos
                if(decodigos.find(codigo_actual) != decodigos.end()){
                    texto_decodificado += decodigos[codigo_actual]; // Decodifica el código actual y agrega el caracter decodificado
                    codigo_actual.clear(); // Limpia el código actual 
                }
            }
            return texto_decodificado; // Retorna el texto decodificado
        }

        // Método para mostrar las frecuencias de los caracteres
        void mostrar_frecuencias(){
            for(auto& pair : frecuencias){
                std::cout << "caracter: " << pair.first << "(" << pair.second << ")" <<std::endl;
            }
            std::cout << std::endl;
        }

        // Método para obtener el tamaño del archivo en KB
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

        // Método para hacer .txt un texto
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

int main(){

    // Abrir el archivo
    std::ifstream archivo("test.txt");
    if(!archivo.is_open()){
        std::cout << "No se pudo abrir el archivo." << std::endl;
        return 1;
    }

    //Leer el contenido en un string
    std::string texto((std::istreambuf_iterator<char>(archivo)), std::istreambuf_iterator<char>());
    archivo.close();

    HuffmanCoder huffman; // Instancia del HuffmanCoder

    //Medir tiempo de codificación
    auto inicio_cod = std::chrono::high_resolution_clock::now();
    std::string codigo = huffman.codificar(texto);
    auto fin_cod = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion_cod = fin_cod - inicio_cod;

    //huffman.mostrar_frecuencias(); // Muestra las frecuencias

    //Medir tiempo de decodificación
    auto inicio_decod = std::chrono::high_resolution_clock::now();
    std::string texto_decodificado = huffman.decodificar(codigo);
    auto fin_decod = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion_decod = fin_decod - inicio_decod;

    //Imprime el texto en todas sus versiones
    //std::cout << "Textos: " << std::endl;
    //std::cout << "Texto original: " << texto << std::endl;
    //std::cout << "Texto codificado: " << codigo << std::endl;
    //std::cout << "Texto decodificado: " << texto_decodificado << std::endl <<std::endl;

    //Transforma la version codificada y decodificada en archivos txt
    huffman.escribir_a_archivo("texto_codificado.txt", codigo);
    huffman.escribir_a_archivo("texto_decodificado.txt", texto_decodificado);

    //calcula los pesos de los archivos txt
    double peso_original = huffman.obtener_peso_archivo("test.txt");
    double peso_codificado = huffman.obtener_peso_archivo("texto_codificado.txt");
    double peso_decodificado = huffman.obtener_peso_archivo("texto_decodificado.txt");

    //Imprime los pesos de los textos
    std::cout << "Tamanos: " << std::endl;
    std::cout << "Peso del texto original (en kilobytes): " << peso_original << " KB" << std::endl;
    std::cout << "Peso del texto codificado (en kilobytes): " << peso_codificado << " KB" << std::endl;
    std::cout << "Peso del texto decodificado (en kilobytes): " << peso_decodificado << " KB" << std::endl << std::endl;

    //Tiempos de compilación
    std::cout << "Tiempos: " << std::endl;
    std::cout << "Tiempo de codificacion: " << duracion_cod.count() << std::endl;
    std::cout << "Tiempo de decodificacion: " << duracion_decod.count() << std::endl;
}
