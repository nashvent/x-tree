# Implementacion de la estructura x-tree en c++
Compilar usando
```
g++ main.cpp -o main
```
Ejecutar 
```
./main
```
Decargar dataset desde : https://archive.ics.uci.edu/ml/datasets/yearpredictionmsd
Descomprimir y usar el nombre de archivo 'YearPredictionMSD.txt'

Analizar CSV: columnas
```
awk -F, '{ print NF; exit }' YearPredictionMSD.txt
```
Analizar CSV: filas
```
cat YearPredictionMSD.txt  | wc -l
```