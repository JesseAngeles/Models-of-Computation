#ifndef TAPE_SYMBOL_H
#define TAPE_SYMBOL_H

#include <string>

class TapeSymbol
{
private:
    // Attribute
    std::string name;

public:
    // Constructor
    TapeSymbol() = default;
    TapeSymbol(const std::string &name) : name(name) {}

    // Overloading
    bool operator<(const TapeSymbol &other) const { return name < other.name; }
    bool operator==(const TapeSymbol &other) const { return name == other.name; }

    // Getters
    std::string getName() const { return name; }

    // Setters
    void setName(const std::string &name) { this->name = name; }
};

#endif // TAPE_SYMBOL_H