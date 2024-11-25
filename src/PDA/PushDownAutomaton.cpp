#include "PDA/PushdownAutomaton.hpp"

// Constructor
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
        {
            init_state = line.substr(11);
        }
        else if (line.rfind("init_stack_symbol:", 0) == 0)
        {
            init_stack_symbol = line.substr(18);
        }
        else if (!line.empty() && line.rfind("transitions:", 0) != 0)
        {
            transitions.push_back(line);
        }
    }

    file.close();

    // Construcción del autómata
    try
    {
        for (const auto &s : states)
            this->states.insert(State(s));

        for (const auto &sym : input_alphabet)
            this->input_alphabet.insert(InputSymbol(sym));

        for (const auto &sym : stack_alphabet)
            this->stack_alphabet.insert(StackSymbol(sym));

        for (const auto &state : this->states)
            if (state.getName() == init_state)
                this->init_state = state;

        for (const auto &stack_symbol : this->stack_alphabet)
            if (stack_symbol.getName() == init_stack_symbol)
                this->init_stack_symbol = stack_symbol;

        for (const auto &state : this->states)
            if (final_states.find(state.getName()) != final_states.end())
                this->final_states.insert(state);

        for (const auto &t_line : transitions)
        {
            Transition transition;

            // Separar por la coma (hay dos partes)
            auto parts = split(t_line, ',');

            // Separar el primer y segundo elemento por ":"
            auto element_before = split(parts[0], ':');
            auto element_after = split(parts[1], ':');

            // Asignar los estados de la transición
            for (const State &state : this->states)
            {
                if (state.getName() == element_before[0])
                    transition.setStartState(state);

                if (state.getName() == element_after[0])
                    transition.setEndState(state);
            }

            if (element_before[1] == "E")
                transition.setSymbol(std::nullopt);
            else
            {
                for (const InputSymbol &sym : this->input_alphabet)
                    if (sym.getName() == element_before[1])
                    {
                        transition.setSymbol(sym);
                        break;
                    }
            }

            for (const StackSymbol &sym : this->stack_alphabet)
                if (sym.getName() == element_before[2])
                    transition.setInitTop(sym);

            // Asignar los símbolos que se empujan en la pila de la transición
            std::stack<StackSymbol> stack_end_top;
            for (int i = element_after.size(); i > 0; i--) // Empezamos en 1 porque el primer elemento es el estado final
                for (const StackSymbol &sym : this->stack_alphabet)
                    if (sym.getName() == element_after[i])
                    {
                        stack_end_top.push(sym); // Empujar el símbolo a la pila
                        break;
                    }

            transition.setEndTop(stack_end_top);

            // Insertar la transición en el conjunto
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

// Displays
void PushdownAutomaton::display() const
{
    std::cout << "\nStates: \n";
    for (const State &state : states)
        std::cout << state.getName() << " ";

    std::cout << "\nInput Alphabet: \n";
    for (const InputSymbol &input_symbol : input_alphabet)
        std::cout << input_symbol.getName() << " ";

    std::cout << "\nStack alphabet: \n";
    for (const StackSymbol &stack_symbol : stack_alphabet)
        std::cout << stack_symbol.getName() << " ";

    std::cout << "\nTransitions: \n";
    for (const Transition &transition : transitions)
    {
        std::cout << "(" << transition.getStartState().getName() << ", "
                  << transition.getEndState().getName() << ") -> ";

        if (transition.getSymbol().has_value())
            std::cout << transition.getSymbol().value().getName();
        else
            std::cout << "E";
        std::cout << "," << transition.getInitTop().getName() << "/";

        if (transition.getEndTop().empty())
            std::cout << "E";
        else
        {
            std::stack<StackSymbol> end_top = transition.getEndTop();
            while (!end_top.empty())
            {
                std::cout << end_top.top().getName() << " ";
                end_top.pop();
            }
        }
        std::cout << "\n";
    }

    std::cout << "\nInit state: " << init_state.getName() << "\n";

    std::cout << "\nInit stack symbol: " << init_stack_symbol.getName() << "\n";

    std::cout << "\nFinal states: \n";
    for (const State &state : final_states)
        std::cout << state.getName() << " ";
}