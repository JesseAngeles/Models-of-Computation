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
    virtual ~Symbol() = default;

    // Overloading
    bool operator<(const Symbol &other) const { return name < other.name; }
    Symbol & operator=(const Symbol &other){
        if (this == &other)
            return *this;

        name = other.name;

        return *this;
    }

    // Getters
    const std::string getName() const { return name; }

    // Setters
    void setName(const std::string &name) { this->name = name; }
};

#endif // SYMBOL_H