#include "../include/FiniteAutomaton.h"

bool FiniteAutomaton::chainValid(string chain)
{
    for (char const &symbol : chain)
        if (this->sigma.find(symbol) == this->sigma.end())
            return false;

    return true;
}

string FiniteAutomaton::trim(const std::string &str)
{
    std::string result;
    for (char c : str)
        if (c != 'm' && c != 't' && std::isalnum(c))
            result += c;

    return result;
}