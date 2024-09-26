#ifndef FINITE_AUTOMATON_H
#define FINITE_AUTOMATON_H

#include <iostream>
#include <set>
#include <tuple>
#include <vector>
#include <string>
#include <sstream>

class FiniteAutomaton
{
private:
    // Variables
    std::set<std::string> q;
    std::set<char> sigma;
    std::set<std::tuple<std::string, char, std::set<std::string>>> delta;
    std::string q_0;
    std::set<std::string> f;
    bool init = false;

    // Asignacion
    void setFromTuple(std::vector<std::vector<std::string>>);
    void setFromMatrix(std::vector<std::vector<std::string>>);

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
    void printVector(std::vector<std::string>);

    // Solution
    bool isChainValid(std::string);
    bool testChain(std::string chain, bool isAccepted = false, std::string currentState = "");

    // Conversion
    void nfa2dfa();
    std::vector<std::vector<std::string>> getInTuple();
    std::vector<std::vector<std::string>> getInMatrix();


    // Getters
    std::set<std::string> get_q() const { return this->q; }
    std::set<char> get_sigma() const { return this->sigma; }
    std::set<std::tuple<std::string, char, std::set<std::string>>> get_delta() const { return this->delta; }
    std::string get_q_0() const { return this->q_0; }
    std::set<std::string> get_f() const { return this->f; }
};

#endif // FINITE_AUTOMATON_H