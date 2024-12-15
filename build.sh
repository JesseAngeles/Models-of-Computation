clear

# Limpiar directorio de compilación
mkdir -p build
rm -rf build/*

# Compilar los archivos fuente
g++ -c -std=c++17 -I./include mainTM.cpp -o build/main.o
g++ -c -std=c++17 -I./include src/TM/TuringMachine.cpp  -o build/TuringMachine.o
g++ -c -std=c++17 -I./include src/TM/Transition.cpp  -o build/Transition.o

# g++ -c -std=c++17 -I./include src/PDA/PushDownAutomaton.cpp  -o build/PushDownAutomaton.o
# g++ -c -std=c++17 -I./include src/PDA/Transition.cpp         -o build/Transition.o
# g++ -c -std=c++17 -I./include src/CFG/ContextFreeGrammar.cpp -o build/ContextFreeGrammar.o
# g++ -c -std=c++17 -I./include src/CFG/ProductionRule.cpp     -o build/ProductionRule.o

# Enlazar los objetos y generar el ejecutable
g++ build/main.o \
    build/TuringMachine.o  build/Transition.o \
    -o build/main.exe
    # build/PushDownAutomaton.o build/Transition.o \
    # build/ContextFreeGrammar.o build/ProductionRule.o \
    

# Ejecutar el programa
./build/main.exe