#ifndef TRANSITION_H
#define TRANSITION_H

#include <optional>
#include <stack>
#include <string>

#include "PDA/State.hpp"
#include "PDA/InputSymbol.hpp"
#include "PDA/StackSymbol.hpp"

class Transition
{
private:
    // Attributes
    State start_state;
    State end_state;
    std::optional<InputSymbol> symbol;
    StackSymbol init_top;
    std::stack<StackSymbol> end_top;

public:
    // Constructor
    Transition() = default;

    Transition(
        const State &start_state,
        const State &end_state,
        const std::optional<InputSymbol> &symbol,
        const StackSymbol &init_top,
        const std::stack<StackSymbol> &end_top);

    Transition(
        const State &start_state,
        const State &end_state,
        const std::optional<InputSymbol> &symbol,
        const StackSymbol &init_top,
        const std::optional<StackSymbol> &end_symbol);

    // Overloading
    bool operator<(const Transition &other) const;

    // Getters
    State getStartState() const { return start_state; }
    State getEndState() const { return end_state; }
    std::optional<InputSymbol> getSymbol() const { return symbol; }
    StackSymbol getInitTop() const { return init_top; }
    std::stack<StackSymbol> getEndTop() const { return end_top; }

    // Setters
    void setStartState(State start_state) { this->start_state = start_state; }
    void setEndState(State end_state) { this->end_state = end_state; }
    void setSymbol(std::optional<InputSymbol> symbol) { this->symbol = symbol; }
    void setInitTop(StackSymbol init_top) { this->init_top = init_top; }
    void setEndTop(std::stack<StackSymbol> end_top) { this->end_top = end_top; }
};

#endif // TRANSITION_H
