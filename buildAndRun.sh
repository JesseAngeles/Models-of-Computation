g++ -c include/FiniteAutomaton.cpp -o build/FiniteAutomaton.o
g++ -c src/main.cpp -o build/practica
g++ -c src/main.cpp -o build/main.o
g++ -o build/practica build/FiniteAutomaton.o build/main.o -Iinclude
./build/practica