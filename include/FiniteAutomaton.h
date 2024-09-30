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
    std::set<std::shared_ptr<State>> q;
    std::set<std::shared_ptr<Symbol>> sigma;
    std::set<std::shared_ptr<Transition>> delta;
    std::shared_ptr<State> q_0;
    std::set<std::shared_ptr<State>> f;

    // Asignacion
    void setFromTuple(std::vector<std::vector<std::string>>);
    void setFromMatrix(std::vector<std::vector<std::string>>);
    void setClosure(std::shared_ptr<Transition> transition);

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
    void printStates();
    void printTransitions();

    // Solution

    // Conversion
    void nfa2dfa();
    std::vector<std::vector<std::string>> getInTuple();
    std::vector<std::vector<std::string>> getInMatrix();

    // Getters
    std::set<std::shared_ptr<State>> get_q() const { return q; }
    std::set<std::shared_ptr<Symbol>> get_sigma() const { return sigma; }
    std::set<std::shared_ptr<Transition>> get_delta() const { return delta; }
    std::shared_ptr<State> get_q_0() const { return q_0; }
    std::set<std::shared_ptr<State>> get_f() const { return f; }
};

#endif // FINITE_AUTOMATON_H