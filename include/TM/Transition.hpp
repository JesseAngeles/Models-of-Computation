#ifndef TRANSITION_H
#define TRANSITION_H

#include <iostream>
#include <memory>
#include <optional>

#include "TM/State.hpp"
#include "TM/Symbol.hpp"
#include "TM/Direction.hpp"

class Transition
{
private:
    // Attributes
    std::shared_ptr<State> start_state;
    std::shared_ptr<State> end_state;
    std::optional<std::shared_ptr<Symbol>> start_symbol;
    std::optional<std::shared_ptr<Symbol>> end_symbol;
    Direction direction;

public:
    // Constructor
    Transition(const std::shared_ptr<State> &start_state,
               const std::shared_ptr<State> &end_state,
               const std::optional<std::shared_ptr<Symbol>> &start_symbol,
               const std::optional<std::shared_ptr<Symbol>> &end_symbol,
               const Direction &direction);

    // Overloading
    bool operator<(const Transition &other) const;

    // Getters
    std::shared_ptr<State> getStartState() const { return start_state; }
    std::shared_ptr<State> getEndState() const { return end_state; }
    std::optional<std::shared_ptr<Symbol>> getStartSymbol() const { return start_symbol; }
    std::optional<std::shared_ptr<Symbol>> getEndSymbol() const { return end_symbol; }
    Direction getDirection() const { return direction; }

    // Setters
    void setStartState(std::shared_ptr<State> start_state) { this->start_state = start_state; }
    void setEndState(std::shared_ptr<State> end_state) { this->end_state = end_state; }
    void setStartSymbol(std::optional<std::shared_ptr<Symbol>> start_symbol) { this->start_symbol = start_symbol; }
    void setEndSymbol(std::optional<std::shared_ptr<Symbol>> end_symbol) { this->end_symbol = end_symbol; }
    void setDirection(Direction direction) { this->direction = direction; }

    // Display
    void display() const;
};

#endif // TRANSITION_H
