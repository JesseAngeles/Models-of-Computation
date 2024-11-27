#ifndef NON_TERMINAL_SYMBOL_H
#define NON_TERMINAL_SYMBOL_H

#include "CFG/Symbol.hpp"

class NonTerminalSymbol : public Symbol
{
private:
public:
    // Constructor
    NonTerminalSymbol(const std::string &name) : Symbol(name) {}
};

#endif // NON_TERMINAL_SYMBOL_H