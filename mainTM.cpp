#include <iostream>
#include <memory>
#include <string>

#include "TM/TuringMachine.hpp"

using namespace std;


int main()
{
    // Numero par de ceros
    
    // Estados
    std::shared_ptr<State> q0 = std::make_shared<State>("q0");
    std::shared_ptr<State> q1 = std::make_shared<State>("q1");

    std::set<std::shared_ptr<State>> states;
    states.insert(q0);
    states.insert(q1);

    // Simbolos de entrada
    std::shared_ptr<Symbol> is0 = std::make_shared<Symbol>("0");
    std::shared_ptr<Symbol> is1 = std::make_shared<Symbol>("1");
    
    std::set<std::shared_ptr<Symbol>> input_alphabet;
    input_alphabet.insert(is0);
    input_alphabet.insert(is1);

    // Simbolos de la cinta
    std::shared_ptr<Symbol> tsx = std::make_shared<Symbol> ("x");

    std::set<std::shared_ptr<Symbol>> tape_alphabet;
    tape_alphabet.insert(tsx);

    // Transiciones
    std::shared_ptr<Transition> t0 = std::make_shared<Transition>(q0, q0, is1, tsx, Direction::Right);
    std::shared_ptr<Transition> t1 = std::make_shared<Transition>(q1, q1, is1, tsx, Direction::Right);
    std::shared_ptr<Transition> t2 = std::make_shared<Transition>(q0, q1, is0, tsx, Direction::Right);
    std::shared_ptr<Transition> t3 = std::make_shared<Transition>(q1, q0, is0, tsx, Direction::Right);
    
    std::set<std::shared_ptr<Transition>> transitions;
    transitions.insert(t0);
    transitions.insert(t1);
    transitions.insert(t2);
    transitions.insert(t3);

    // Final states
    std::set<std::shared_ptr<State>> final_states;
    final_states.insert(q0);

    TuringMachine tm(states, input_alphabet, tape_alphabet, transitions, q0, final_states);
    tm.display();
}