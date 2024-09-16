#!/bin/bash

# Compilar los archivos de implementación en archivos objeto
g++ -c -Iinclude/classes -Iinclude/structs src/main.cpp -o main.o
g++ -c -Iinclude/classes -Iinclude/structs src/dfa.cpp -o fda.o

# Compilar el archivo principal y enlazar
g++ -o practica1 main.o fda.o -Iinclude -L.

# Ejecutar el programa
./practica1
