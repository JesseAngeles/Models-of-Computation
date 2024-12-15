#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>

class Symbol
{
private:
    // Attributes
    std::string name;

public:
    // Constructor
    Symbol(const std::string &name) : name(name) {}

    // Getters
    std::string getName() const { return name; }

    // Overloading
    bool operator<(const Symbol other) const { return name < other.name; }
    bool operator==(const Symbol other) const { return name == other.name; }

    // Setters
    void setName(const std::string &name) { this->name = name; }
};

#endif // SYMBOL_H