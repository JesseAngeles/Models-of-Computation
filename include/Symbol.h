#ifndef SYMBOL_H
#define SYMBOL_H

#include <iostream>

class Symbol
{
public:
    Symbol(std::string name) : name(name) {}

    // getters
    std::string getName() const { return name; }

    // setters
    void setName(std::string name) { this->name = name; }

    // Funciones
    bool isEpsilon() const { return name == "E"; }

    // Sobrecarga del operador <
    bool operator<(const Symbol &other) const
    {
        return name < other.name;
    }

    // Sobrecarga del operador ==
    bool operator==(const Symbol &other) const
    {
        return name == other.name;
    }

private:
    std::string name;
};

#endif // SYMBOL_H