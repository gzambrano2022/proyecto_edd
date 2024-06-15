#!/bin/bash

# Nombre del ejecutable
ejecutable="./huffman"

# Archivo donde se guardarán los tiempos de ejecución
archivo_csv="tiempos_ejecucion_huffman.csv"

# Número de ejecuciones
num_ejecuciones=20

# Crear o vaciar el archivo CSV y escribir la cabecera
echo "Ejecucion,Tiempo_Codificacion,Tiempo_Decodificacion" > $archivo_csv

# Ejecutar el programa y capturar los tiempos de ejecución
for ((i=1; i<=num_ejecuciones; i++))
do
    # Ejecutar el programa y capturar la salida
    resultado=$($ejecutable)
    
    # Extraer los tiempos de la salida
    tiempo_codificacion=$(echo "$resultado" | grep "Tiempo de codificacion" | awk '{print $4}')
    tiempo_decodificacion=$(echo "$resultado" | grep "Tiempo de decodificacion" | awk '{print $4}')
    
    # Escribir los tiempos de ejecución en el archivo CSV
    echo "$i,$tiempo_codificacion,$tiempo_decodificacion" >> $archivo_csv
done

echo "Los tiempos de ejecución se han guardado en $archivo_csv"
