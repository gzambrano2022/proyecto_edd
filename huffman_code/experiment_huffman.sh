#!/bin/bash

# Nombre del ejecutable
ejecutable="./huffman"

# Carpeta de entrada
carpeta_entrada="../texts"

# Archivo donde se guardarán los tiempos de ejecución
archivo_csv="tiempos_ejecucion_huffman.csv"

# Número de ejecuciones
num_ejecuciones=20

# Crear o vaciar el archivo CSV y escribir la cabecera
echo "Archivo,Tiempo_Codificacion,Tiempo_Decodificacion,Peso_Original,Peso_Codificado" > $archivo_csv

# Lista de archivos en la carpeta de entrada
archivos=("dna1MB.txt" "dna5MB.txt" "dna10MB.txt" "dna20MB.txt" "dna50MB.txt")
archivos=("dna1MB.txt" "dna5MB.txt" "dna10MB.txt" "dna20MB.txt" "dna50MB.txt")
archivos=("dna1MB.txt" "dna5MB.txt" "dna10MB.txt" "dna20MB.txt" "dna50MB.txt")

# Ejecutar el programa para cada archivo y capturar los tiempos de ejecución
for archivo in "${archivos[@]}"
do
    for ((i=1; i<=num_ejecuciones; i++))
    do
        # Ejecutar el programa y capturar la salida
        resultado=$($ejecutable "$carpeta_entrada/$archivo")
        
        # Extraer los tiempos de la salida
        tiempo_codificacion=$(echo "$resultado" | grep "Tiempo de codificacion" | awk '{print $4}')
        tiempo_decodificacion=$(echo "$resultado" | grep "Tiempo de decodificacion" | awk '{print $4}')
        peso_original=$(echo "$resultado" | grep "Peso del texto original" | awk '{print $7}')
        peso_codificado=$(echo "$resultado" | grep "Peso del texto codificado" | awk '{print $7}')
        
        # Escribir los tiempos de ejecución en el archivo CSV
        echo "$archivo,$tiempo_codificacion,$tiempo_decodificacion,$peso_original,$peso_codificado" >> $archivo_csv
    done
done

echo "Los tiempos de ejecución se han guardado en $archivo_csv"
