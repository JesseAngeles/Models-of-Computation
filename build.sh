clear

# Limpiar directorio de compilación
mkdir -p build
rm -rf build/*

# Compilar los archivos fuente
g++ -c -std=c++17 -I./include main.cpp -o build/main.o
g++ -c -std=c++17 -I./include src/PDA/PushDownAutomaton.cpp  -o build/PushDownAutomaton.o
g++ -c -std=c++17 -I./include src/PDA/Transition.cpp         -o build/Transition.o
g++ -c -std=c++17 -I./include src/CFG/ContextFreeGrammar.cpp -o build/ContextFreeGrammar.o

# Enlazar los objetos y generar el ejecutable
g++ build/main.o \
    build/PushDownAutomaton.o build/Transition.o \
    build/ContextFreeGrammar.o \
    -o build/main.exe

# Ejecutar el programa
./build/main.exe