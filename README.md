# proyecto_edd
- Joaquín Godoy
- Benjamín Villarroel
- Gabriela Zambrano

Los textos utilizados para el análisis experimental fueron extraidos de https://pizzachili.dcc.uchile.cl/texts.html

# Experimentos:
Tanto para Huffman como para Lempel Ziv se realizaron pruebas con archivos de 50mb (English, DNA, ...).
Para el algoritmo de codificación de Huffman se ejecutaron las pruebas 20 veces y se calculó su promedio.

# Ejecución:
Para realizar pruebas de forma manual, se deben seguir los siguientes pasos:

1. Ambos programas leen un archivo "text.txt" perteneciente a su respectiva carpeta, por lo que si se desea realizar pruebas con distintos tipos de texto, se recomienda modificar el archivo agregando el texto deseado. Alternativamente, puede copiar uno de los textos propuestos de la carpeta texts, pegarlo en la carpeta donde se va a ejecutar el código (huffman_code o lempelZiv_code), y cambiar su nombre a "text.txt". Así el programa podrá leerlo.

2. Una vez hecho el paso anterior, compilar el programa y ejecutarlo escribiendo lo siguiente en la terminal:
- Para Huffman:
```
g++ huffman_v2.cpp -o huffman
./huffman
```
- Para Lempel Ziv:
```
g++ lempel_ziv.cpp -o lzv
./lzv
```
Los resultados de ejecución del programa serán desplegados en la salida de la terminal. Luego, se guardaran los archivos en la carpeta results asociada a cada código