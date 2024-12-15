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
    this->blank = std::make_shared<Symbol>("B");
    this->tape_alphabet.insert(blank);
    this->tape_alphabet.insert(input_alphabet.begin(), input_alphabet.end());
}

TuringMachine::TuringMachine(const std::string &file_path)
{
    std::ifstream file(file_path);
    if (!file.is_open())
        return;

    std::string line;
    std::string init_state;
    std::set<std::string> states, input_alphabet, tape_alphabet, final_states;
    std::vector<std::string> transitions;

    // Reading
    while (std::getline(file, line))
    {
        if (line.rfind("states:", 0) == 0)
        {
            auto tokens = split(line.substr(7), ',');
            states = std::set<std::string>(tokens.begin(), tokens.end());
        }
        else if (line.rfind("input_alphabet:", 0) == 0)
        {
            auto tokens = split(line.substr(15), ',');
            input_alphabet = std::set<std::string>(tokens.begin(), tokens.end());
        }
        else if (line.rfind("tape_alphabet:", 0) == 0)
        {
            auto tokens = split(line.substr(14), ',');
            tape_alphabet = std::set<std::string>(tokens.begin(), tokens.end());
        }
        else if (line.rfind("final_states:", 0) == 0)
        {
            auto tokens = split(line.substr(13), ',');
            final_states = std::set<std::string>(tokens.begin(), tokens.end());
        }
        else if (line.rfind("init_state:", 0) == 0)
            init_state = line.substr(11);
        else if (!line.empty() && line.rfind("transitions:", 0) != 0)
            transitions.push_back(line);
    }

    file.close();

    // Writting

    this->blank = std::make_shared<Symbol>("B");
    this->tape_alphabet.insert(blank);

    // States
    for (const std::string &state : states)
    {
        std::shared_ptr<State> new_state = std::make_shared<State>(state);
        // Find coincidences
        if (this->states.find(new_state) == this->states.end())
            this->states.insert(new_state);
    }

    // input alphabet
    for (const std::string &symbol : input_alphabet)
    {
        std::shared_ptr<Symbol> new_symbol = std::make_shared<Symbol>(symbol);
        // Find coincidences
        if (this->input_alphabet.find(new_symbol) == this->input_alphabet.end())
        {
            this->input_alphabet.insert(new_symbol);
            this->tape_alphabet.insert(new_symbol);
        }
    }

    // tape alphabet
    for (const std::string &symbol : tape_alphabet)
    {
        std::shared_ptr<Symbol> new_symbol = std::make_shared<Symbol>(symbol);
        // Find coincidences
        if (this->tape_alphabet.find(new_symbol) == this->tape_alphabet.end())
            this->tape_alphabet.insert(new_symbol);
    }

    // Final states
    for (const std::string &final_state : final_states)
    {
        std::shared_ptr<State> new_final_state = std::make_shared<State>(final_state);
        // Find coincidences
        for (const std::shared_ptr<State> &state : this->states)
            if (state->getName() == new_final_state->getName())
            {
                this->final_states.insert(state);
                break;
            }
    }

    // Init state
    for (const std::shared_ptr<State> &state : this->states)
        if (state->getName() == init_state)
            this->init_state = state;

    // Transitions
    for (const std::string &transition : transitions)
    {
        auto parts = split(transition, ',');
        auto from = split(parts[0], ':');
        auto to = split(parts[1], ':');

        // States
        std::shared_ptr<State> from_state, to_state;
        for (const std::shared_ptr<State> &state : this->states)
        {
            if (state->getName() == from[0])
                from_state = state;
            if (state->getName() == to[0])
                to_state = state;
        }

        // Symbol
        std::shared_ptr<Symbol> init_symbol, end_symbol;

        for (const std::shared_ptr<Symbol> &symbol : this->tape_alphabet)
        {
            if (symbol->getName() == from[1])
                init_symbol = symbol;
            if (symbol->getName() == to[1])
                end_symbol = symbol;
        }

        // Direction
        Direction direction = (to[2] == "L") ? Direction::Left : Direction::Right;

        // Join
        std::shared_ptr<Transition> new_transition =
            std::make_shared<Transition>(from_state, to_state, init_symbol, end_symbol, direction);

        // Find coincidences
        if (this->transitions.find(new_transition) == this->transitions.end())
            this->transitions.insert(new_transition);
    }
}

// Private functions
std::vector<std::string> TuringMachine::split(const std::string &line, char delimiter)
{
    std::stringstream ss(line);
    std::string item;
    std::vector<std::string> tokens;
    while (std::getline(ss, item, delimiter))
        tokens.push_back(item);
    return tokens;
}

bool TuringMachine::isChainValid(std::deque<std::shared_ptr<Symbol>> &chain) const
{
    // Iterate every symbol
    for (const std::shared_ptr<Symbol> &symbol : chain)
        if (input_alphabet.find(symbol) == input_alphabet.end())
            return false;

    return true;
}

bool TuringMachine::recursiveTest(
    std::deque<std::shared_ptr<Symbol>> &chain,
    std::shared_ptr<State> &current_state,
    int &current_index)
{
    // Alcanzo un estado de aceptacion
    if (final_states.find(current_state) != final_states.end())
        return true;

    // Buscar transiciones
    for (const std::shared_ptr<Transition> &transition : transitions)
    {
        // Si conicide el estado inicial y el simbolo inicial
        if (transition->getStartState() == current_state &&
            transition->getStartSymbol() == chain.at(current_index))
        {
            // new chain
            std::deque<std::shared_ptr<Symbol>> new_chain = chain;
            new_chain[current_index] = transition->getEndSymbol();

            // new index
            int new_index = current_index + ((transition->getDirection() == Direction::Left) ? -1 : 1);

            if (new_index < 0)
            {
                new_chain.push_front(blank);
                new_index++;
            }

            if (new_index >= static_cast<int>(new_chain.size()))
                new_chain.push_back(blank);

            // new state
            std::shared_ptr<State> new_current_state = transition->getEndState();

            if (recursiveTest(new_chain, new_current_state, new_index))
                return true;
        }
    }

    // No hay mas movimiento posibles
    return false;
}

// Public functions
std::deque<std::shared_ptr<Symbol>> TuringMachine::createChain(const std::vector<std::string> &string_chain)
{
    std::deque<std::shared_ptr<Symbol>> symbol_chain;

    // Iterate every string
    for (const std::string &element : string_chain)
        // Iterate every symbol
        for (const std::shared_ptr<Symbol> &symbol : input_alphabet)
            if (symbol->getName() == element)
            {
                symbol_chain.push_back(symbol);
                break;
            }

    return symbol_chain;
}

bool TuringMachine::testChain(std::deque<std::shared_ptr<Symbol>> &chain)
{
    if (!isChainValid(chain))
        return false;

    chain.push_front(blank);
    chain.push_back(blank);

    int current_index = 1;
    std::shared_ptr<State> current_state = init_state;

    return recursiveTest(chain, current_state, current_index);

}

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