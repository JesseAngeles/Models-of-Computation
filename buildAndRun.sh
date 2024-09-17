g++ -c src/FiniteAutomaton.cpp -o build/FiniteAutomaton.o
g++ -c src/dfa.cpp -o build/dfa.o
g++ -c src/nfa.cpp -o build/nfa.o
g++ -c src/main.cpp -o build/main.o
g++ -o build/practica1 build/FiniteAutomaton.o build/dfa.o build/nfa.o build/main.o -Iinclude
./build/practica1