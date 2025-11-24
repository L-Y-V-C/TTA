# TTA

Trending Topic Analyzer

## Descripción del Proyecto

Sistema de análisis en tiempo real de trending topics utilizando algoritmo de stemming de Lovins y R*-tree espacial para indexación temporal-frecuencial, y hash tables personalizadas. El sistema procesa noticias, extrae palabras clave mediante stemming, las indexa en un R*-tree 2D (tiempo × frecuencia), y genera word clouds dinámicos en Python.
Dependencias

## Librerías de python necesarias:

- pandas
- matplotlib
- wordcloud

## Configuración inicial

En el archivo “config.txt” se podrá configurar manualmente los parámetros iniciales los cuales son los siguientes:

- batchSize: Cantidad de archivos a procesar en el lote inicial
- intervalSec: Tiempo de espera entre procesamiento de archivos individuales
- topK: Número de trending topics a exportar al CSV
- lastMNews: Tamaño de la ventana temporal (últimas M noticias procesadas)
- maxWordsPerFile: Máximo de palabras con igual frecuencia a extraer por archivo

Se configuran solo cambiando el número al lado del parámetro separado de un espacio siguiendo en siguiente formato:

batchSize 2000
intervalSec 1
topK 35
lastMNews 150
maxWordsPerFile 3

## Conjunto de datos

En directorio que contiene las noticias a procesar debe encontrarse en el mismo directorio del archivo main.cpp y cambiarse el nombre en la linea 34 del mismo archivo.

tta.loadNewsFiles("Carpeta con las noticias");

Adicionalmente, se proporciona un directorio con conjunto de datos con el nombre “AP_BBC_CNN” en un archivo comprimido.

## Compilación

comando a ejecutar para iniciar el programa:

g++ -std=c++17 -O3 -I. main.cpp cpp/lovins.cpp cpp/rtree.cpp cpp/tta.cpp cpp/pool.cpp cpp/rect.cpp cpp/topicwindow.cpp -o tta

Este comando compila los archivos .cpp y generará un ejecutable llamado “tta”.

Se podrá ejecutar el proyecto con el comando “./tta” ó “tta”.

# Estructura

Se usa un R*-Tree bidimensional, donde cada palabra se representa como un punto en un espacio de coordenadas (tiempo, frecuencia). El eje X corresponde al tiempo incremental, mientras que el eje Y representa la frecuencia acumulada de cada palabra en ese momento específico, esta representación espacial permite realizar búsquedas eficientes sobre ventanas temporales y rangos de frecuencia mediante consultas rectangulares, además se usan dos tablas hash complementarias que mantienen la correspondencia entre palabras y sus identificadores.

A continuación vemos una representación de las estructuras:

<img width="536" height="610" alt="image" src="https://github.com/user-attachments/assets/25a60ce4-54ee-42d7-a7a6-b4b0a7b71352" />
