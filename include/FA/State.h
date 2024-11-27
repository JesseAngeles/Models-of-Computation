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
    State(std::string name) : name(name) { isClosureSet = false; }

    // Getters
    std::string getName() const { return name; }
    std::set<std::shared_ptr<State>> getClosure() const { return closure; }
    bool getIsClosureSet() const { return isClosureSet; }

    // Setters
    void setName(std::string name) { this->name = name; }
    void setIsClosureSet(bool isClosureSet) { this->isClosureSet = isClosureSet; }

    // Insert
    void insertClosure(std::shared_ptr<State> state) { this->closure.insert(state); }

    // Sobrecarga del operador <
    bool operator<(const State &other) const { return name < other.name; }

private:
    std::string name;
    std::set<std::shared_ptr<State>> closure;
    bool isClosureSet;
};

#endif // STATE_H
