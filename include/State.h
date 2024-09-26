#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <set>
#include <memory>
#include <string>

class State : public std::enable_shared_from_this<State>
{
public:
    State() = default;
    State(std::string name) : name(name) {}

    // Getters
    std::string getName() const { return name; }

    // Setters
    void setName(std::string name) { this->name = name; }

    // Insert
    void insertClosure(std::shared_ptr<State> &state) { this->closure.insert(state); }

    // Sobrecarga del operador <
    bool operator<(const State &other) const
    {
        return name < other.name;
    }

    // Sobrecarga del operador ==
    bool operator==(const State &other) const
    {
        return name == other.name; // Comparar estados por nombre
    }

    // Sobrecarga del operador !
    bool operator!() const
    {
        return name.empty();
    }

private:
    std::string name;
    std::set<std::shared_ptr<State>> closure;
};

#endif // STATE_H
