#ifndef DFA_H
#define DFA_H

#include <sstream>

#include "./FiniteAutomaton.h"

using namespace std;

class DFA : public FiniteAutomaton
{
private:
    set<tuple<string, char, string>> delta;
    void setFromMatrix(vector<vector<string>> matrix);
    void setFromTuple(vector<vector<string>> matrix);
    tuple<string, string, string> splitString(const string &str);
public:
    DFA() = default;
    DFA(const vector<vector<string>> &matrix);
    void printMatrix();
    void printTuple();
    bool chainValid(string);
    bool testChain(string);
};

#endif // DFA_H
