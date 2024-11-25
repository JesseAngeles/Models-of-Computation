#ifndef TRANSITION_H
#define TRANSITION_H

#include <iostream>
#include <set>
#include <memory>

#include "State.h"
#include "Symbol.h"

class Transition
{
public:
    Transition(std::shared_ptr<State> exitState, std::shared_ptr<Symbol> symbol, std::set<std::shared_ptr<State>> arrivalStates)
        : exitState(exitState), symbol(symbol), arrivalStates(arrivalStates) {}

    // Getters
    std::shared_ptr<State> getExitState() const { return exitState; }
    std::shared_ptr<Symbol> getSymbol() const { return symbol; }
    std::set<std::shared_ptr<State>> &getArrivalStates() { return arrivalStates; }

    // Setters
    void setExitState(std::shared_ptr<State> exitState) { this->exitState = exitState; }
    void setArrivalStates(std::set<std::shared_ptr<State>> arrivalState) { this->arrivalStates = arrivalState; }

    // Insert
    void insertArrivalState(std::shared_ptr<State> state) { arrivalStates.insert(state); }

    // Sobrecarga del operador <
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
    std::shared_ptr<State> exitState;
    std::shared_ptr<Symbol> symbol;
    std::set<std::shared_ptr<State>> arrivalStates;
};

#endif // TRANSITION_H