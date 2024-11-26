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
#include <memory>

#include "PDA/State.hpp"
#include "PDA/InputSymbol.hpp"
#include "PDA/StackSymbol.hpp"
#include "PDA/Transition.hpp"

#include "PDA/auxVisited.hpp"

class PushdownAutomaton
{
private:
    // Attributes
    std::set<std::shared_ptr<State>> states;
    std::set<std::shared_ptr<InputSymbol>> input_alphabet;
    std::set<std::shared_ptr<StackSymbol>> stack_alphabet;
    std::set<std::shared_ptr<Transition>> transitions;
    std::shared_ptr<State> init_state;
    std::shared_ptr<StackSymbol> init_stack_symbol;
    std::set<std::shared_ptr<State>> final_states;

    // Functions
    std::vector<std::string> split(const std::string &line, char delimiter);
    bool isChainValid(const std::string &chain) const;
    std::vector<std::shared_ptr<InputSymbol>> string2vector(const std::string &chain);
    void pushStack(std::stack<std::shared_ptr<StackSymbol>> &stack,
                   std::stack<std::shared_ptr<StackSymbol>> top);
    bool recursiveTest(std::shared_ptr<State> current_state,
                       std::stack<std::shared_ptr<StackSymbol>> current_stack,
                       std::vector<std::shared_ptr<InputSymbol>> current_chain,
                       std::set<auxVisited> visited);

public:
    // Constructor
    PushdownAutomaton(
        const std::set<std::shared_ptr<State>> &states,
        const std::set<std::shared_ptr<InputSymbol>> &input_alphabet,
        const std::set<std::shared_ptr<StackSymbol>> &stack_alphabet,
        const std::set<std::shared_ptr<Transition>> &transitions,
        const std::shared_ptr<State> &init_state,
        const std::shared_ptr<StackSymbol> &init_stack_symbol,
        const std::set<std::shared_ptr<State>> &final_states);

    PushdownAutomaton(const std::string &file_path);

    // Functions
    bool testChain(std::string);

    // Getters
    const std::set<std::shared_ptr<State>> &getStates() const;
    const std::set<std::shared_ptr<InputSymbol>> &getInputAlphabet() const;
    const std::set<std::shared_ptr<StackSymbol>> &getStackAlphabet() const;
    const std::set<std::shared_ptr<Transition>> &getTransitions() const;
    const std::shared_ptr<State> &getInitState() const;
    const std::shared_ptr<StackSymbol> &getInitStackSymbol() const;
    const std::set<std::shared_ptr<State>> &getFinalStates() const;

    // Setters
    void setStates(const std::set<std::shared_ptr<State>> &states);
    void setInputAlphabet(const std::set<std::shared_ptr<InputSymbol>> &input_alphabet);
    void setStackAlphabet(const std::set<std::shared_ptr<StackSymbol>> &stack_alphabet);
    void setTransitions(const std::set<std::shared_ptr<Transition>> &transitions);
    void setInitState(const std::shared_ptr<State> &init_state);
    void setInitStackSymbol(const std::shared_ptr<StackSymbol> &init_stack_symbol);
    void setFinalStates(const std::set<std::shared_ptr<State>> &final_states);

    // Display
    void display() const;
    void displayStep(std::shared_ptr<State> current_state,
                     std::stack<std::shared_ptr<StackSymbol>> current_stack,
                     std::vector<std::shared_ptr<InputSymbol>> current_chain) const;
};

#endif // PUSH_DOWN_AUTOMATON_H