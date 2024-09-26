#ifndef TRANSITION_H
#define TRANSITION_H

#include <iostream>
#include <set>

#include "State.h"
#include "Symbol.h"

class Transition
{
public:
    Transition() = default;
    Transition(State exitState, Symbol symbol, std::set<State> arrivalStates)
        : exitState(exitState), symbol(symbol), arrivalStates(arrivalStates) {}

    // Getters
    State getExitState() const { return exitState; }
    Symbol getSymbol() const { return symbol; }
    std::set<State> getArrivalStates() const { return arrivalStates; }

    // Setters
    void setExitState(State exitState) { this->exitState = exitState; }

    // Insert
    void insertArrivalState(std::set<State> states) { arrivalStates.insert(states.begin(), states.end()); }

    void setArrivalStates(std::set<State> arrivalState) { this->arrivalStates = arrivalState; }

    bool operator<(const Transition &other) const
    {
        if (exitState == other.exitState)
        {
            if (symbol == other.symbol)
                return arrivalStates < other.arrivalStates;
            return symbol < other.symbol;
        }
        return exitState < other.exitState;
    }

private:
    State exitState;
    Symbol symbol;
    std::set<State> arrivalStates;
};

#endif // TRANSITION_H