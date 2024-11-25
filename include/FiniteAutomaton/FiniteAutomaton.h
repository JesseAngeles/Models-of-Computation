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

    std::string type;

    // Asignation
    void setFromTuple(std::vector<std::vector<std::string>>);
    void setFromMatrix(std::vector<std::vector<std::string>>);
    void setClosure(std::shared_ptr<Transition> transition);

    // Identify initial and final states
    bool is_q_0(std::string &);
    bool is_f(std::string &);

    // Aditiona functions
    std::vector<std::string> splitString(std::string);
    std::string trim(std::string);
    bool findCycle(std::shared_ptr<State> initialState, std::shared_ptr<State> currentState);

public:
    // Constructor
    FiniteAutomaton() = default;
    FiniteAutomaton(const std::vector<std::vector<std::string>> &);

    // Print
    void printTuple();
    void printMatrix();
    void printStates();

    // Solution
    bool isChainValid(std::vector<std::string> chain);
    bool testChain(std::vector<std::string> chain, bool final = false, std::shared_ptr<State> currentState = nullptr);

    // Conversion
    void nfae2nfa();
    void nfa2dfa();
    std::shared_ptr<State> closure2state(std::shared_ptr<State>);
    std::shared_ptr<State> set2State(std::set<std::shared_ptr<State>>);
    std::vector<std::vector<std::string>> getInTuple();
    std::vector<std::vector<std::string>> getInMatrix();

    // Getters
    std::set<std::shared_ptr<State>> get_q() const { return q; }
    std::set<std::shared_ptr<Symbol>> get_sigma() const { return sigma; }
    std::set<std::shared_ptr<Transition>> get_delta() const { return delta; }
    std::shared_ptr<State> get_q_0() const { return q_0; }
    std::set<std::shared_ptr<State>> get_f() const { return f; }

    std::string getType() const { return type; }
};

#endif // FINITE_AUTOMATON_H