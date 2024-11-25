#ifndef PUSH_DOWN_AUTOMATON_H
#define PUSH_DOWN_AUTOMATON_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include <stack>
#include <optional>
#include <string>
#include <iostream>

#include "PDA/State.hpp"
#include "PDA/InputSymbol.hpp"
#include "PDA/StackSymbol.hpp"
#include "PDA/Transition.hpp"

class PushdownAutomaton
{
private:
    // Attributes
    std::set<State> states;
    std::set<InputSymbol> input_alphabet;
    std::set<StackSymbol> stack_alphabet;
    std::set<Transition> transitions;
    State init_state;
    StackSymbol init_stack_symbol;
    std::set<State> final_states;

    // Functions
    std::vector<std::string> split(const std::string &line, char delimiter);

public:
    // Constructor
    PushdownAutomaton(
        const std::set<State> &states,
        const std::set<InputSymbol> &input_alphabet,
        const std::set<StackSymbol> &stack_alphabet,
        const std::set<Transition> &transitions,
        const State &init_state,
        const StackSymbol &init_stack_symbol,
        const std::set<State> &final_states)
        : states(states),
          input_alphabet(input_alphabet),
          stack_alphabet(stack_alphabet),
          transitions(transitions),
          init_state(init_state),
          init_stack_symbol(init_stack_symbol),
          final_states(final_states) {}

    PushdownAutomaton(const std::string &file_path);

    // Functions

    // Getters
    const std::set<State> &getStates() const { return states; }
    const std::set<InputSymbol> &getInputAlphabet() const { return input_alphabet; }
    const std::set<StackSymbol> &getStackAlphabet() const { return stack_alphabet; }
    const std::set<Transition> &getTransition() const { return transitions; }
    const State &getInitState() const { return init_state; }
    const StackSymbol &getInitStackSymbol() const { return init_stack_symbol; }
    const std::set<State> &getFinalStates() const { return final_states; }

    // Setters
    void setStates(const std::set<State> &states) { this->states = states; }
    void setInputAlphabet(const std::set<InputSymbol> &input_alphabet) { this->input_alphabet = input_alphabet; }
    void setStackAlphabet(const std::set<StackSymbol> &stack_alphabet) { this->stack_alphabet = stack_alphabet; }
    void setTransition(const std::set<Transition> &transitions) { this->transitions = transitions; }
    void setStates(const State &init_state) { this->init_state = init_state; }
    void setInitStackAlphabet(const StackSymbol &init_stack_symbol) { this->init_stack_symbol = init_stack_symbol; }
    void setFinalStates(const std::set<State> &final_states) { this->final_states = final_states; }

    // Displays
    void display() const;
};

#endif // PUSH_DOWN_AUTOMATON_H