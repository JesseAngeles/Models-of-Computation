#ifndef TRANSITION_H
#define TRANSITION_H

#include <optional>
#include <stack>
#include <string>
#include <memory>
#include <iostream>

#include "PDA/State.hpp"
#include "PDA/InputSymbol.hpp"
#include "PDA/StackSymbol.hpp"

class Transition
{
private:
    // Attributes
    std::shared_ptr<State> start_state;
    std::shared_ptr<State> end_state;
    std::optional<std::shared_ptr<InputSymbol>> symbol;
    std::shared_ptr<StackSymbol> init_top;
    std::stack<std::shared_ptr<StackSymbol>> end_top;

public:
    // Constructor
    Transition() = default;

    Transition(
        const std::shared_ptr<State> &start_state,
        const std::shared_ptr<State> &end_state,
        const std::optional<std::shared_ptr<InputSymbol>> &symbol,
        const std::shared_ptr<StackSymbol> &init_top,
        const std::stack<std::shared_ptr<StackSymbol>> &end_top);

    Transition(
        const std::shared_ptr<State> &start_state,
        const std::shared_ptr<State> &end_state,
        const std::optional<std::shared_ptr<InputSymbol>> &symbol,
        const std::shared_ptr<StackSymbol> &init_top,
        const std::optional<std::shared_ptr<StackSymbol>> &end_symbol);

    // Overloading
    bool operator<(const Transition &other) const;

    // Getters
    std::shared_ptr<State> getStartState() const { return start_state; }
    std::shared_ptr<State> getEndState() const { return end_state; }
    std::optional<std::shared_ptr<InputSymbol>> getSymbol() const { return symbol; }
    std::shared_ptr<StackSymbol> getInitTop() const { return init_top; }
    std::stack<std::shared_ptr<StackSymbol>> getEndTop() const { return end_top; }

    // Setters
    void setStartState(std::shared_ptr<State> start_state) { this->start_state = start_state; }
    void setEndState(std::shared_ptr<State> end_state) { this->end_state = end_state; }
    void setSymbol(std::optional<std::shared_ptr<InputSymbol>> symbol) { this->symbol = symbol; }
    void setInitTop(std::shared_ptr<StackSymbol> init_top) { this->init_top = init_top; }
    void setEndTop(std::stack<std::shared_ptr<StackSymbol>> end_top) { this->end_top = end_top; }

    // Display
    void display() const;
};

#endif // TRANSITION_H
