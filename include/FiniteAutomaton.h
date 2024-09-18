#ifndef FINITE_AUTOMATON_H
#define FINITE_AUTOMATON_H

#include <iostream>
#include <set>
#include <tuple>
#include <vector>
#include <string>

using namespace std;

class FiniteAutomaton
{
protected:
    set<string> q;
    set<char> sigma;
    string q_0;
    set<string> f;
    bool init = false;

    FiniteAutomaton() = default;

    bool isInit() const { return this->init; }

    string trim(const string &);

public:
    bool chainValid(string);

    // Getters
    set<string> get_q() const { return this->q; }
    set<char> get_sigma() const { return this->sigma; }
    string get_q_0() const { return this->q_0; }
    set<string> get_f() const { return this->f; }
};

#endif // FINITE_AUTOMATON_H