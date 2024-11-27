#ifndef TERMINAL_SYMBOL_H
#define TERMINAL_SYMBOL_H

#include "CFG/Symbol.hpp"

class TerminalSymbol : public Symbol
{
private:
public:
    // Constructor
    TerminalSymbol(const std::string &name) : Symbol(name) {}
};

#endif // TERMINAL_SYMBOL_H