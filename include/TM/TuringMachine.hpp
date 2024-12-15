#ifndef TURING_MACHINE_H
#define TURING_MACHINE_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>
#include <deque>
#include <memory>

#include "TM/State.hpp"
#include "TM/Symbol.hpp"
#include "TM/Transition.hpp"

class TuringMachine
{
private:
    // Attributes
    std::set<std::shared_ptr<State>> states;
    std::set<std::shared_ptr<Symbol>> input_alphabet;
    std::set<std::shared_ptr<Symbol>> tape_alphabet;
    std::set<std::shared_ptr<Transition>> transitions;
    std::shared_ptr<State> init_state;
    std::set<std::shared_ptr<State>> final_states;
    std::shared_ptr<Symbol> blank;

    // Functions
    std::vector<std::string> split(const std::string &line, char delimiter);

    bool isChainValid(std::deque<std::shared_ptr<Symbol>> &chain) const;
    bool recursiveTest(
        std::deque<std::shared_ptr<Symbol>> &chain,
        std::shared_ptr<State> &current_state,
        int &current_index);

public:
    // Constructor
    TuringMachine() = default;

    TuringMachine(
        const std::set<std::shared_ptr<State>> &states,
        const std::set<std::shared_ptr<Symbol>> &input_alphabet,
        const std::set<std::shared_ptr<Symbol>> &tape_alphabet,
        const std::set<std::shared_ptr<Transition>> &transitions,
        const std::shared_ptr<State> &init_state,
        const std::set<std::shared_ptr<State>> &final_states);

    TuringMachine(const std::string &file_path);

    // Functions
    std::deque<std::shared_ptr<Symbol>> createChain(const std::vector<std::string> &string_chain);
    bool testChain(std::deque<std::shared_ptr<Symbol>> &chain);

    // Getters
    const std::set<std::shared_ptr<State>> &getStates() const { return states; }
    const std::set<std::shared_ptr<Symbol>> &getInputAlphabet() const { return input_alphabet; }
    const std::set<std::shared_ptr<Symbol>> &getTapeAlphabet() const { return tape_alphabet; }
    const std::set<std::shared_ptr<Transition>> &getTransitions() const { return transitions; }
    std::shared_ptr<State> getInitState() const { return init_state; }
    const std::set<std::shared_ptr<State>> &getFinalStates() const { return final_states; }
    std::shared_ptr<Symbol> getBlank() const { return blank; }

    // Setters
    void setStates(const std::set<std::shared_ptr<State>> &newStates) { states = newStates; }
    void setInputAlphabet(const std::set<std::shared_ptr<Symbol>> &newInputAlphabet) { input_alphabet = newInputAlphabet; }
    void setTapeAlphabet(const std::set<std::shared_ptr<Symbol>> &newTapeAlphabet) { tape_alphabet = newTapeAlphabet; }
    void setTransitions(const std::set<std::shared_ptr<Transition>> &newTransitions) { transitions = newTransitions; }
    void setInitState(const std::shared_ptr<State> &newInitState) { init_state = newInitState; }
    void setFinalStates(const std::set<std::shared_ptr<State>> &newFinalStates) { final_states = newFinalStates; }
    void setBlank(const std::shared_ptr<Symbol> &newBlank) { blank = newBlank; }

    // Display
    void display() const;
};

#endif // TURING_MACHINE_H