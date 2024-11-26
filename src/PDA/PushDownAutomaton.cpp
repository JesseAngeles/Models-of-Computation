#include "PDA/PushdownAutomaton.hpp"

// Constructors
PushdownAutomaton::PushdownAutomaton(
    const std::set<std::shared_ptr<State>> &states,
    const std::set<std::shared_ptr<InputSymbol>> &input_alphabet,
    const std::set<std::shared_ptr<StackSymbol>> &stack_alphabet,
    const std::set<std::shared_ptr<Transition>> &transitions,
    const std::shared_ptr<State> &init_state,
    const std::shared_ptr<StackSymbol> &init_stack_symbol,
    const std::set<std::shared_ptr<State>> &final_states)
    : states(states),
      input_alphabet(input_alphabet),
      stack_alphabet(stack_alphabet),
      transitions(transitions),
      init_state(init_state),
      init_stack_symbol(init_stack_symbol),
      final_states(final_states) {}

PushdownAutomaton::PushdownAutomaton(const std::string &file_path)
{
    std::ifstream file(file_path);
    if (!file.is_open())
        throw std::runtime_error("No se pudo abrir el archivo.");

    std::string line;
    std::string init_state, init_stack_symbol;
    std::set<std::string> states, input_alphabet, stack_alphabet, final_states;
    std::vector<std::string> transitions;

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
        else if (line.rfind("stack_alphabet:", 0) == 0)
        {
            auto tokens = split(line.substr(15), ',');
            stack_alphabet = std::set<std::string>(tokens.begin(), tokens.end());
        }
        else if (line.rfind("final_states:", 0) == 0)
        {
            auto tokens = split(line.substr(13), ',');
            final_states = std::set<std::string>(tokens.begin(), tokens.end());
        }
        else if (line.rfind("init_state:", 0) == 0)
            init_state = line.substr(11);
        else if (line.rfind("init_stack_symbol:", 0) == 0)
            init_stack_symbol = line.substr(18);
        else if (!line.empty() && line.rfind("transitions:", 0) != 0)
            transitions.push_back(line);
    }

    file.close();

    // Construcción del autómata
    try
    {
        // Crear estados
        for (const auto &s : states)
            this->states.insert(std::make_shared<State>(s));

        // Crear alfabeto de entrada
        for (const auto &sym : input_alphabet)
            this->input_alphabet.insert(std::make_shared<InputSymbol>(sym));

        // Crear alfabeto de pila
        for (const auto &sym : stack_alphabet)
            this->stack_alphabet.insert(std::make_shared<StackSymbol>(sym));

        // Asignar estado inicial
        for (const auto &state : this->states)
            if (state->getName() == init_state)
                this->init_state = state;

        // Asignar símbolo inicial de pila
        for (const auto &stack_symbol : this->stack_alphabet)
            if (stack_symbol->getName() == init_stack_symbol)
                this->init_stack_symbol = stack_symbol;

        // Crear estados finales
        for (const auto &state : this->states)
            if (final_states.find(state->getName()) != final_states.end())
                this->final_states.insert(state);

        // Crear transiciones
        for (const auto &t_line : transitions)
        {
            auto parts = split(t_line, ',');
            auto element_before = split(parts[0], ':');
            auto element_after = split(parts[1], ':');

            std::shared_ptr<State> start_state, end_state;
            for (const auto &state : this->states)
            {
                if (state->getName() == element_before[0])
                    start_state = state;
                if (state->getName() == element_after[0])
                    end_state = state;
            }

            std::optional<std::shared_ptr<InputSymbol>> symbol;
            if (element_before[1] == "E")
                symbol = std::nullopt;
            else
                for (const auto &input_sym : this->input_alphabet)
                    if (input_sym->getName() == element_before[1])
                    {
                        symbol = input_sym;
                        break;
                    }

            std::shared_ptr<StackSymbol> top_symbol;
            for (const auto &stack_sym : this->stack_alphabet)
                if (stack_sym->getName() == element_before[2])
                    top_symbol = stack_sym;

            std::stack<std::shared_ptr<StackSymbol>> stack_end_top;
            for (int i = element_after.size() - 1; i > 0; --i)
                for (const auto &stack_sym : this->stack_alphabet)
                    if (stack_sym->getName() == element_after[i])
                    {
                        stack_end_top.push(stack_sym);
                        break;
                    }

            auto transition = std::make_shared<Transition>(start_state, end_state, symbol, top_symbol, stack_end_top);
            this->transitions.insert(transition);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error al construir el autómata: " << e.what() << std::endl;
    }
}

// Private functions
std::vector<std::string> PushdownAutomaton::split(const std::string &line, char delimiter)
{
    std::stringstream ss(line);
    std::string item;
    std::vector<std::string> tokens;
    while (std::getline(ss, item, delimiter))
        tokens.push_back(item);
    return tokens;
}

bool PushdownAutomaton::isChainValid(const std::string &chain) const
{
    for (const char &c : chain)
    {
        bool found = false;
        for (const auto &symbol : input_alphabet)
            if (symbol->getName().size() == 1 && symbol->getName()[0] == c)
            {
                found = true;
                break;
            }

        if (!found)
            return false;
    }

    return true;
}

std::vector<std::shared_ptr<InputSymbol>> PushdownAutomaton::string2vector(const std::string &chain)
{
    std::vector<std::shared_ptr<InputSymbol>> new_chain;

    for (const char c : chain)
        for (const std::shared_ptr<InputSymbol> &symbol : input_alphabet)
            if (symbol->getName() == std::string(1, c))
            {
                new_chain.push_back(symbol);
                break;
            }

    return new_chain;
}

void PushdownAutomaton::pushStack(std::stack<std::shared_ptr<StackSymbol>> &stack,
                                  std::stack<std::shared_ptr<StackSymbol>> top)
{
    std::stack<std::shared_ptr<StackSymbol>> reverse;
    while (!top.empty())
    {
        reverse.push(top.top());
        top.pop();
    }

    while(!reverse.empty())
    {
        stack.push(reverse.top());
        reverse.pop();
    }
}

bool PushdownAutomaton::recursiveTest(
    std::shared_ptr<State> current_state,
    std::stack<std::shared_ptr<StackSymbol>> current_stack,
    std::vector<std::shared_ptr<InputSymbol>> current_chain)
{
    if (final_states.empty() && current_stack.empty())
        return current_chain.empty();

    // Tomar el símbolo actual (si existe)
    std::optional<std::shared_ptr<InputSymbol>> current_symbol =
        current_chain.empty() ? std::nullopt : std::make_optional(current_chain.front());

    // Iterar sobre las funciones de transición
    for (const std::shared_ptr<Transition> &transition : transitions)
    {
        // Validar si la transición es aplicable
        if (transition->getStartState() == current_state &&
            transition->getInitTop() == current_stack.top())
        {

            std::vector<std::shared_ptr<InputSymbol>> next_chain = current_chain;
            std::shared_ptr<State> next_state = transition->getEndState();
            std::stack<std::shared_ptr<StackSymbol>> next_stack = current_stack;

            if (transition->getSymbol().has_value())
            {
                if (!current_symbol.has_value())
                    continue;

                if (transition->getSymbol().value() == current_symbol.value())
                    next_chain.erase(next_chain.begin());
                else
                    continue;
            }

            // Actualizar la pila
            next_stack.pop();
            pushStack(next_stack, transition->getEndTop());

            // Llamada recursiva con las actualizaciones
            if (recursiveTest(next_state, next_stack, next_chain))
                return true;
        }
    }

    // Condición de salida: verificar si la cadena ya fue leída
    if (current_chain.empty())
        for (const std::shared_ptr<State> &state : final_states)
            if (*state == *current_state)
                return true;

    // Si ninguna transición llevó a aceptación, retornar falso
    return false;
}

// Public functions
bool PushdownAutomaton::testChain(std::string chain)
{
    if (!isChainValid(chain))
        return false;

    std::shared_ptr<State> current_state = init_state;
    std::stack<std::shared_ptr<StackSymbol>> current_stack;
    current_stack.push(init_stack_symbol);
    std::vector<std::shared_ptr<InputSymbol>> current_chain = string2vector(chain);

    return recursiveTest(current_state, current_stack, current_chain);
}

// Displays
void PushdownAutomaton::display() const
{
    std::cout << "\nStates: \n";
    for (const std::shared_ptr<State> &state : states)
        std::cout << state->getName() << " ";

    std::cout << "\nInput Alphabet: \n";
    for (const std::shared_ptr<InputSymbol> &input_symbol : input_alphabet)
        std::cout << input_symbol->getName() << " ";

    std::cout << "\nStack alphabet: \n";
    for (const std::shared_ptr<StackSymbol> &stack_symbol : stack_alphabet)
        std::cout << stack_symbol->getName() << " ";

    std::cout << "\nTransitions: \n";
    for (const std::shared_ptr<Transition> &transition : transitions)
    {
        std::cout << "(" << transition->getStartState()->getName() << ", "
                  << transition->getEndState()->getName() << ") -> ";

        if (transition->getSymbol().has_value())
            std::cout << transition->getSymbol().value()->getName();
        else
            std::cout << "E";
        std::cout << "," << transition->getInitTop()->getName() << "/";

        if (transition->getEndTop().empty())
            std::cout << "E";
        else
        {
            std::stack<std::shared_ptr<StackSymbol>> end_top = transition->getEndTop();
            while (!end_top.empty())
            {
                std::cout << end_top.top()->getName() << " ";
                end_top.pop();
            }
        }
        std::cout << "\n";
    }

    std::cout << "\nInit state: " << init_state->getName() << "\n";

    std::cout << "\nInit stack symbol: " << init_stack_symbol->getName() << "\n";

    std::cout << "\nFinal states: \n";
    for (const std::shared_ptr<State> &state : final_states)
        std::cout << state->getName() << " ";
}

void PushdownAutomaton::displayStep(
    std::shared_ptr<State> current_state,
    std::stack<std::shared_ptr<StackSymbol>> current_stack,
    std::vector<std::shared_ptr<InputSymbol>> current_chain) const
{
    std::cout << "\nCurrent state: " << current_state->getName();

    std::cout << "\nCurrent stack: ";
    while (!current_stack.empty())
    {
        std::cout << current_stack.top()->getName() << " ";
        current_stack.pop();
    }

    std::cout << "\nCurrent chain: ";
    for (std::shared_ptr<InputSymbol> sym : current_chain)
        std::cout << sym->getName() << " ";

    std::cout << std::endl;
}