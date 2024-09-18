#ifndef DFA_H
#define DFA_H

#include <sstream>
#include <algorithm>

#include "./FiniteAutomaton.h"
#include "./nfa.h"

using namespace std;

class DFA : public FiniteAutomaton
{
private:
    set<tuple<string, char, string>> delta;
    void setFromMatrix(vector<vector<string>> matrix);
    void setFromTuple(vector<vector<string>> matrix);
    tuple<string, string, string> splitString(const string &str);
    string setToString(set<string> info);

public:
    DFA() = default;
    DFA(const vector<vector<string>> &matrix);
    void printMatrix();
    void printTuple();
    bool chainValid(string);
    bool testChain(string);

    void nfa2dfa(NFA nfa);
};

#endif // DFA_H
