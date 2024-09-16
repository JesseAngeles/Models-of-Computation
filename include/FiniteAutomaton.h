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
};

#endif // FINITE_AUTOMATON_H