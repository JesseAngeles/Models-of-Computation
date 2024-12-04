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
#include <algorithm>

#include "PDA/State.hpp"
#include "PDA/InputSymbol.hpp"
#include "PDA/StackSymbol.hpp"
#include "PDA/Transition.hpp"
#include "CFG/ContextFreeGrammar.hpp"

#include "PDA/auxVisited.hpp"

class ContextFreeGrammar;

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
    std::string makeString(const std::shared_ptr<State> &, const std::shared_ptr<StackSymbol> &, const std::shared_ptr<State> &);
    std::vector<int> changeBase(int number, const int &base, const int &size);

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
    const bool isByEmptySet() const { return final_states.empty(); }

    void empty_set2final_state();
    void final_state2empty_set();

    void toCFG();

    // Getters
    const std::set<std::shared_ptr<State>> &getStates() const { return states; }
    const std::set<std::shared_ptr<InputSymbol>> &getInputAlphabet() const { return input_alphabet; }
    const std::set<std::shared_ptr<StackSymbol>> &getStackAlphabet() const { return stack_alphabet; }
    const std::set<std::shared_ptr<Transition>> &getTransitions() const { return transitions; }
    const std::shared_ptr<State> &getInitState() const { return init_state; }
    const std::shared_ptr<StackSymbol> &getInitStackSymbol() const { return init_stack_symbol; }
    const std::set<std::shared_ptr<State>> &getFinalStates() const { return final_states; }

    // Setters
    void setStates(const std::set<std::shared_ptr<State>> &states) { this->states = states; }
    void setInputAlphabet(const std::set<std::shared_ptr<InputSymbol>> &input_alphabet) { this->input_alphabet = input_alphabet; }
    void setStackAlphabet(const std::set<std::shared_ptr<StackSymbol>> &stack_alphabet) { this->stack_alphabet = stack_alphabet; }
    void setTransitions(const std::set<std::shared_ptr<Transition>> &transitions) { this->transitions = transitions; }
    void setInitState(const std::shared_ptr<State> &init_state) { this->init_state = init_state; }
    void setInitStackSymbol(const std::shared_ptr<StackSymbol> &init_stack_symbol) { this->init_stack_symbol = init_stack_symbol; }
    void setFinalStates(const std::set<std::shared_ptr<State>> &final_states) { this->final_states = final_states; }

    // Display
    void display() const;
    void displayStep(std::shared_ptr<State> current_state,
                     std::stack<std::shared_ptr<StackSymbol>> current_stack,
                     std::vector<std::shared_ptr<InputSymbol>> current_chain) const;
};

#endif // PUSH_DOWN_AUTOMATON_H