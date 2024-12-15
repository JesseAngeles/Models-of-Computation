#ifndef INPUT_SYMBOL_H
#define INPUT_SYMBOL_H

#include <string>

class InputSymbol
{
private:
    // Attributes
    std::string name;

public:
    // Constructor
    InputSymbol() = default;

    InputSymbol(const std::string &name)
        : name(name) {}

    // Overloading
    bool operator<(const InputSymbol &other) const { return name < other.name; }

    // Getters
    std::string getName() const { return name; }

    // Setters
    void setName(const std::string &name) { this->name = name; }
};

#endif // INPUT_SYMBOL_H