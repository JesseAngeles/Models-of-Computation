#ifndef FINITE_AUTOMATON_H
#define FINITE_AUTOMATON_H

#include <iostream>
#include <set>
#include <tuple>
#include <vector>
#include <string>
#include <sstream>

#include "State.h"
#include "Symbol.h"
#include "Transition.h"

class FiniteAutomaton
{
private:
    // Variables
    std::set<State> q;
    std::set<Symbol> sigma;
    std::set<Transition> delta;
    State q_0;
    std::set<State> f;

    // Asignacion
    void setFromTuple(std::vector<std::vector<std::string>>);
    void setFromMatrix(std::vector<std::vector<std::string>>);
    void setClosures(State &);

    // Identify initial and final states
    bool is_q_0(std::string &);
    bool is_f(std::string &);

    // Aditiona functions
    std::vector<std::string> splitString(std::string);
    std::string trim(std::string);

public:
    // Constructor
    FiniteAutomaton() = default;
    FiniteAutomaton(std::vector<std::vector<std::string>> &);

    // Print
    void printTuple();
    void printMatrix();

    // Solution
    bool findEpsilonCicle();

    // Conversion
    void nfa2dfa();
    std::vector<std::vector<std::string>> getInTuple();
    std::vector<std::vector<std::string>> getInMatrix();

    // Getters
    std::set<State> get_q() const { return q; }
    std::set<Symbol> get_sigma() const { return sigma; }
    std::set<Transition> get_delta() const { return delta; }
    State get_q_0() const { return q_0; }
    std::set<State> get_f() const { return f; }
};

#endif // FINITE_AUTOMATON_H