#ifndef NON_TERMINAL_SYMBOL_H
#define NON_TERMINAL_SYMBOL_H

#include "CFG/Symbol.hpp"

class NonTerminalSymbol : public Symbol
{
private:
    std::string name;

public:
    // Constructor
    NonTerminalSymbol(const std::string &name) : Symbol(name) {}

    bool operator<(const NonTerminalSymbol &other) const { return name < other.name; }
};

#endif // NON_TERMINAL_SYMBOL_H