#!/bin/bash

# Nombre del ejecutable
ejecutable="./lz"

# Carpeta de entrada
carpeta_entrada="../texts"

# Archivo donde se guardarán los tiempos de ejecución
archivo_csv="tiempos_ejecucion_lz_english.csv"

# Número de ejecuciones
num_ejecuciones=20

# Crear o vaciar el archivo CSV y escribir la cabecera
echo "Archivo,Tiempo_Compresion,Tiempo_Descompresion,Peso_Original,Peso_Comprimido" > $archivo_csv

# Lista de archivos en la carpeta de entrada (descomentar lo que se desee ejecutar)
#archivos=("dna1MB.txt" "dna5MB.txt" "dna10MB.txt" "dna20MB.txt" "dna50MB.txt")
#archivos=("proteins1MB.txt" "proteins5MB.txt" "proteins10MB.txt" "proteins20MB.txt" "proteins50MB.txt")
archivos=("english1MB.txt" "english5MB.txt" "english10MB.txt" "english20MB.txt" "english50MB.txt")

# Ejecutar el programa para cada archivo y capturar los tiempos de ejecución
for archivo in "${archivos[@]}"
do
    for ((i=1; i<=num_ejecuciones; i++))
    do
        # Ejecutar el programa y capturar la salida
        resultado=$($ejecutable "$carpeta_entrada/$archivo")
        
        # Extraer los tiempos de la salida
        tiempo_compresion=$(echo "$resultado" | grep "Tiempo de compresion" | awk '{print $4}')
        tiempo_descompresion=$(echo "$resultado" | grep "Tiempo de decompresion" | awk '{print $4}')
        peso_original=$(echo "$resultado" | grep "Peso del texto original" | awk '{print $7}')
        peso_comprimido=$(echo "$resultado" | grep "Peso del texto comprimido" | awk '{print $7}')
        
        # Escribir los tiempos de ejecución en el archivo CSV
        echo "$archivo,$tiempo_compresion,$tiempo_descompresion,$peso_original,$peso_comprimido" >> $archivo_csv
    done
done

echo "Los tiempos de ejecución se han guardado en $archivo_csv"
