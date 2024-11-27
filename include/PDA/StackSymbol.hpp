#ifndef STACK_SYMBOL_H
#define STACK_SYMBOL_H

#include <string>

class StackSymbol
{
private:
    // Attributes
    std::string name;

public:
    // Constructor
    StackSymbol(const std::string &name)
        : name(name) {}
    StackSymbol() : name("z0") {}

    // Overloading
    bool operator<(const StackSymbol &other) const { return name < other.name; }

    // Getters
    std::string getName() const { return name; }

    // Setters
    void setName(const std::string &name) { this->name = name; }
};

#endif // STACK_SYMBOL_H