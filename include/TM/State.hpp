#ifndef STATE_H
#define STATE_H

#include <string>

class State
{
private:
    // Attributes
    std::string name;

public:
    // Constructor
    State() = default;
    State(const std::string &name)
        : name(name) {};

    // Overloading
    bool operator<(const State &other) const { return name < other.name; }
    bool operator==(const State &other) const { return name == other.name; }

    // Getters
    std::string getName() const { return name; }

    // Setters
    void setName(const std::string &name) { this->name = name; }
};

#endif // STATE_H