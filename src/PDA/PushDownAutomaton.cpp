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
        return;

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

    while (!reverse.empty())
    {
        stack.push(reverse.top());
        reverse.pop();
    }
}

bool PushdownAutomaton::recursiveTest(
    std::shared_ptr<State> current_state,
    std::stack<std::shared_ptr<StackSymbol>> current_stack,
    std::vector<std::shared_ptr<InputSymbol>> current_chain,
    std::set<auxVisited> visited)
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

            // Añadir a los visitados
            auxVisited new_visited(next_state, next_chain.size());
            if (!next_stack.empty())
                new_visited.setSymbol(next_stack.top());
            if (visited.find(new_visited) != visited.end())
                continue;
            else
                visited.insert(new_visited);

            // Llamada recursiva con las actualizaciones
            if (recursiveTest(next_state, next_stack, next_chain, visited))
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

std::string PushdownAutomaton::makeString(const std::shared_ptr<State> &from,
                                          const std::shared_ptr<StackSymbol> &symbol,
                                          const std::shared_ptr<State> &to)
{
    return "[" + from->getName() + " " + symbol->getName() + " " + to->getName() + "]";
}

std::vector<int> PushdownAutomaton::changeBase(int number, const int &base, const int &size)
{
    std::vector<int> res;
    if (number == 0)
    {
        res.assign(size, 0);
        return res;
    }

    while (number > 0)
    {
        int residue = number % base;
        res.push_back(residue);
        number /= base;
    }

    std::reverse(res.begin(), res.end());

    if (res.size() < size)
    {
        size_t zeros_to_add = size - res.size();
        res.insert(res.begin(), zeros_to_add, 0);
    }
    else if (res.size() > size)
    {
        // Truncar si es más grande
        res = std::vector<int>(res.end() - size, res.end());
    }

    return res;
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

    std::set<auxVisited> visited;
    visited.insert(auxVisited(current_state, current_chain.size(), current_stack.top()));

    return recursiveTest(current_state, current_stack, current_chain, visited);
}

void PushdownAutomaton::toFinalStates()
{
    // Create new states
    std::shared_ptr<State> new_init_state = std::make_shared<State>("a");
    std::shared_ptr<State> new_final_state = std::make_shared<State>("end");

    // create new init stack
    std::shared_ptr<StackSymbol> new_init_stack_symbol = std::make_shared<StackSymbol>("x0");

    // Crear nueva stack
    std::stack<std::shared_ptr<StackSymbol>> new_init_stack;
    new_init_stack.push(new_init_stack_symbol);
    new_init_stack.push(init_stack_symbol);

    // New first transition
    std::shared_ptr<Transition> new_init_transition =
        std::make_shared<Transition>(new_init_state, init_state, std::nullopt, new_init_stack_symbol, new_init_stack);

    // Transitions to final state
    for (const std::shared_ptr<State> &state : states)
    {
        std::shared_ptr<Transition> new_transition =
            std::make_shared<Transition>(state, new_final_state, std::nullopt, new_init_stack_symbol, new_init_stack_symbol);

        transitions.insert(new_transition);
    }

    // Set elements
    states.insert(new_init_state);
    states.insert(new_final_state);

    init_state = new_init_state;
    final_states.insert(new_final_state);

    stack_alphabet.insert(new_init_stack_symbol);
    init_stack_symbol = new_init_stack_symbol;

    transitions.insert(new_init_transition);
}

void PushdownAutomaton::toEmptyStack()
{
    // Create new state
    std::shared_ptr<State> new_final_state = std::make_shared<State>("end");
    states.insert(new_final_state);
    final_states.insert(new_final_state);

    // Create new transition
    for (const std::shared_ptr<State> &state : final_states)
        for (const std::shared_ptr<StackSymbol> &symbol : stack_alphabet)
        {
            std::shared_ptr<Transition> new_transition =
                std::make_shared<Transition>(state, new_final_state, std::nullopt, symbol, std::nullopt);

            transitions.insert(new_transition);
        }

    final_states.clear();
}

ContextFreeGrammar PushdownAutomaton::toCFG()
{
    // Estado inicial
    std::shared_ptr<NonTerminalSymbol> start_state = std::make_shared<NonTerminalSymbol>("S");
    // std::set<std::shared_ptr<ProductionRule>> start_productions;
    std::set<std::shared_ptr<TerminalSymbol>> terminal_alphabet;
    std::set<std::shared_ptr<NonTerminalSymbol>> non_terminal_alphabet;
    std::set<std::shared_ptr<ProductionRule>> productions;

    std::shared_ptr<StackSymbol> stack_epsilon = std::make_shared<StackSymbol>("E"); // Variable auxiliar

    non_terminal_alphabet.insert(start_state);

    // Definir todo el alfabeto de simbolos no terminales = alfabeto de entrada
    for (const std::shared_ptr<InputSymbol> &input_symbol : input_alphabet)
        terminal_alphabet.insert(std::make_shared<TerminalSymbol>(input_symbol->getName()));

    // For cada simbolo se crea una nueva transicion
    for (const std::shared_ptr<State> &state : states)
    {
        std::shared_ptr<Symbol> new_symbol = std::make_shared<Symbol>(makeString(init_state, init_stack_symbol, state));
        std::vector<std::shared_ptr<Symbol>> symbol;

        non_terminal_alphabet.insert(std::static_pointer_cast<NonTerminalSymbol>(new_symbol));
        symbol.push_back(new_symbol);
        std::shared_ptr<ProductionRule> prod = std::make_shared<ProductionRule>(start_state, symbol);
        productions.insert(prod);
    }

    // Iterar para cada transicion
    for (const std::shared_ptr<Transition> &transition : transitions)
    {
        // Buscar el simbolo de transicion en los simbolos terminales
        std::shared_ptr<TerminalSymbol> transition_symbol;
        for (const std::shared_ptr<TerminalSymbol> &terminal_symbol : terminal_alphabet)
        {
            if (transition->getSymbol().has_value() &&
                transition->getSymbol().value()->getName() == terminal_symbol->getName())
                transition_symbol = terminal_symbol;
        }

        // Calcular el numero de producciones
        int stack_size = transition->getEndTop().size();
        if (!stack_size)
            stack_size = 1;

        int total_productions = std::pow(states.size(), stack_size);

        std::vector<std::shared_ptr<State>> vector_states(states.begin(), states.end());
        std::stack<std::shared_ptr<StackSymbol>> stack_copy = transition->getEndTop();
        std::vector<std::shared_ptr<StackSymbol>> stack_vector;
        while (!stack_copy.empty())
        {
            stack_vector.push_back(stack_copy.top());
            stack_copy.pop();
        }

        // Generar las combinaciones de producciones
        std::vector<std::vector<int>> combinatorial;
        for (int i = 0; i < total_productions; i++)
            combinatorial.push_back(changeBase(i, states.size(), stack_size));

        for (const std::vector<int> &comb : combinatorial)
        {
            std::shared_ptr<State> last_state = transition->getEndState();

            std::vector<std::shared_ptr<Symbol>> production;
            if (transition_symbol)
                production.push_back(transition_symbol);

            for (int i = 0; i < comb.size(); i++)
            {
                std::string prod_name;
                if (!stack_vector.empty())
                    prod_name = makeString(last_state, stack_vector[i], vector_states[comb[i]]);
                else if (last_state == vector_states[comb[i]])
                {
                    continue;
                }
                else
                {
                    prod_name = makeString(last_state, stack_epsilon, vector_states[comb[i]]);
                }

                std::shared_ptr<NonTerminalSymbol> prod_symbol = std::make_shared<NonTerminalSymbol>(prod_name);

                // Encontrar prod_name en non_terminal_alphabet
                bool finded = false;
                for (const std::shared_ptr<NonTerminalSymbol> &non_terminal_symbol : non_terminal_alphabet)
                {
                    if (non_terminal_symbol->getName() == prod_name)
                    {
                        finded = true;
                        prod_symbol = non_terminal_symbol;
                        break;
                    }
                }

                if (!finded)
                    non_terminal_alphabet.insert(prod_symbol);

                production.push_back(prod_symbol);

                last_state = vector_states[comb[i]];
            }

            std::string prod_name = makeString(transition->getStartState(), transition->getInitTop(), last_state);
            // std::cout << " <- " << prod_name;

            std::shared_ptr<NonTerminalSymbol> prod_symbol = std::make_shared<NonTerminalSymbol>(prod_name);
            // Encontrar prod_name en non_terminal_alphabet
            bool finded = false;
            for (const std::shared_ptr<NonTerminalSymbol> &non_terminal_symbol : non_terminal_alphabet)
            {
                if (non_terminal_symbol->getName() == prod_name)
                {
                    finded = true;
                    prod_symbol = non_terminal_symbol;
                    break;
                }
            }

            if (!finded)
                non_terminal_alphabet.insert(prod_symbol);

            std::shared_ptr<ProductionRule> new_production = std::make_shared<ProductionRule>(prod_symbol, production);
            productions.insert(new_production);
        }
    }

    // Renombrar non_terminal
    char name = 'A';
    for (const std::shared_ptr<NonTerminalSymbol> &symbol : non_terminal_alphabet)
    {
        if (symbol->getName() == "S")
            continue;
        if (name == 'E')
            name++;
        std::string new_name = "";
        new_name += name++;
        symbol->setName(new_name);
    }

    return ContextFreeGrammar(non_terminal_alphabet, terminal_alphabet, productions, start_state);
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
    
    std::cout << std::endl;
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