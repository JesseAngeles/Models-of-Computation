#include "TM/TuringMachine.hpp"

// Constructor
TuringMachine::TuringMachine(
    const std::set<std::shared_ptr<State>> &states,
    const std::set<std::shared_ptr<Symbol>> &input_alphabet,
    const std::set<std::shared_ptr<Symbol>> &tape_alphabet,
    const std::set<std::shared_ptr<Transition>> &transitions,
    const std::shared_ptr<State> &init_state,
    const std::set<std::shared_ptr<State>> &final_states)
    : states(states),
      input_alphabet(input_alphabet),
      tape_alphabet(tape_alphabet),
      transitions(transitions),
      init_state(init_state),
      final_states(final_states)
{
    // Add input-alphabet to tape-alphabet
    this->tape_alphabet.insert(input_alphabet.begin(), input_alphabet.end());
}

// Private functions

// Public functions

// Display

void TuringMachine::display() const
{
    std::cout << "\nStates: \n";
    for (const std::shared_ptr<State> &state : states)
        std::cout << state->getName() << " ";

    std::cout << "\nInput alphabet: \n";
    for (const std::shared_ptr<Symbol> &input_symbol : input_alphabet)
        std::cout << input_symbol->getName() << " ";

    std::cout << "\nTape alphabet: \n";
    for (const std::shared_ptr<Symbol> &tape_symbol : tape_alphabet)
        std::cout << tape_symbol->getName() << " ";

    std::cout << "\nTransition: \n";
    for (const std::shared_ptr<Transition> &transition : transitions)
        transition->display();

    std::cout << "\nInit State: " << init_state->getName();

    std::cout << "\nFinal states: \n";
    for (const std::shared_ptr<State> &final_state : final_states)
        std::cout << final_state->getName() << " ";

    std::cout << std::endl;
}