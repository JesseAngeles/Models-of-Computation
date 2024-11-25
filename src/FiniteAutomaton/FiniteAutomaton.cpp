#include "FiniteAutomaton.h"

FiniteAutomaton::FiniteAutomaton(const std::vector<std::vector<std::string>> &af)
{
    if (af[0][0] == "t")
        setFromTuple(af);
    else
        setFromMatrix(af);

    // Establecer la clausura
    std::set<std::shared_ptr<Transition>> delta_epsilon;
    std::set<std::shared_ptr<Transition>> delta_normal;

    // Obtenemos las transiciones que contienen a epsilon
    for (const std::shared_ptr<Transition> &transition : delta)
        if (transition->getSymbol()->isEpsilon())
            delta_epsilon.insert(transition);
        else
            delta_normal.insert(transition);

    // Establecer la clausuara de los estados sin transiciones con epsilon (q[E] -> q)
    for (const std::shared_ptr<Transition> &transition : delta_normal)
        transition->getExitState()->setIsClosureSet(true);

    // Establecer la clausuara de los estados sin transiciones con epsilon (q_0[E] -> q_1)
    for (const std::shared_ptr<Transition> &transition : delta_epsilon)
        setClosure(transition);

    // Establecer el tipo
    type = "dfa";
    for (const std::shared_ptr<Transition> &transition : delta)
        if (transition->getSymbol()->isEpsilon())
            type = "nfae";
        else if (transition->getArrivalStates().size() > 1)
            type = "nfa";
}

// Establecer los valores
void FiniteAutomaton::setFromTuple(std::vector<std::vector<std::string>> tuple)
{
    // Establecer q
    for (int j = 1; j < tuple[0].size(); j++)
    {
        State q_i(trim(tuple[0][j]));
        std::shared_ptr<State> q_i_ptr = std::make_shared<State>(q_i);
        q.insert(q_i_ptr);
    }

    // Establecer sigma
    for (int j = 0; j < tuple[1].size(); j++)
    {
        Symbol sigma_i(trim(tuple[1][j]));
        std::shared_ptr<Symbol> sigma_i_ptr = std::make_shared<Symbol>(sigma_i);
        sigma.insert(sigma_i_ptr);
    }

    // Establecer delta
    for (int j = 0; j < tuple[2].size(); j++)
    {
        // Obtemos la información de la transición
        std::vector<std::string> data = splitString(tuple[2][j]);

        std::shared_ptr<State> q_exit_ptr = nullptr;
        std::shared_ptr<Symbol> sigma_q_ptr = nullptr;
        std::shared_ptr<State> q_arrival_ptr = nullptr;

        // Validación de existencia
        for (const std::shared_ptr<State> &state : q)
        {
            if (state->getName() == trim(data[0]))
                q_exit_ptr = state;
            if (state->getName() == trim(data[2]))
                q_arrival_ptr = state;
        }

        for (const std::shared_ptr<Symbol> &symbol : sigma)
            if (symbol->getName() == trim(data[1]))
            {
                sigma_q_ptr = symbol;
                break;
            }

        if (!q_exit_ptr || !sigma_q_ptr || !q_arrival_ptr)
        {
            std::cout << "Error: uno o más punteros son nulos" << std::endl;
            return; // Detener si algún puntero es nulo
        }

        // se busca si la transición ya existe
        bool transitionExists = false;
        for (const std::shared_ptr<Transition> &transition : delta)
            if (transition->getExitState() == q_exit_ptr && transition->getSymbol() == sigma_q_ptr)
            {
                std::set<std::shared_ptr<State>> &arrivalStates = transition->getArrivalStates();
                arrivalStates.insert(q_arrival_ptr);
                transitionExists = true;
                break;
            }

        if (!transitionExists)
        {
            Transition newTransition(q_exit_ptr, sigma_q_ptr, {q_arrival_ptr});
            delta.insert(std::make_shared<Transition>(newTransition));
        }
    }

    // Establecer q_0
    State q_0_tuple(trim(tuple[3][0]));
    for (const std::shared_ptr<State> &state : q)
        if (state->getName() == q_0_tuple.getName())
        {
            q_0 = state;
            break;
        }

    // Establecer f
    for (int j = 0; j < tuple[4].size(); j++)
    {
        State q_tuple(trim(tuple[4][j]));
        for (const std::shared_ptr<State> &state : q)
            if (state->getName() == q_tuple.getName())
            {
                f.insert(state);
                break;
            }
    }
}

void FiniteAutomaton::setFromMatrix(std::vector<std::vector<std::string>> matrix)
{
    // Estabelcer q, q_0 y f
    for (int i = 1; i < matrix.size(); i++)
    {
        std::string q_current = matrix[i][0];
        std::shared_ptr<State> q_ptr = std::make_shared<State>(State(trim(q_current)));

        std::shared_ptr<State> q_0_ptr = nullptr;
        std::shared_ptr<State> q_f_ptr = nullptr;

        bool q_0_flag = false;
        bool f_flag = false;

        if (is_q_0(q_current) && q_0 == nullptr)
            q_0_flag = true;

        if (is_f(q_current))
            f_flag = true;

        // Inserciones
        if (q_0_flag)
            q_0 = q_ptr;
        if (f_flag)
            f.insert(q_ptr);
        q.insert(q_ptr);

        matrix[i][0] = trim(q_current);
    }

    // Establecer sigma
    for (int j = 1; j < matrix[0].size(); j++)
        sigma.insert(std::make_shared<Symbol>(Symbol(trim(matrix[0][j]))));

    // Establecer delta
    for (int i = 1; i < matrix.size(); i++)
        for (int j = 1; j < matrix[i].size(); j++)
        {
            if (matrix[i][j].empty())
                continue;

            std::shared_ptr<State> exitState = nullptr;
            std::shared_ptr<Symbol> symbol = nullptr;
            std::set<std::shared_ptr<State>> arrivalStates;

            std::vector<std::string> arrivalVector = splitString(matrix[i][j]);

            // Establecer exitState
            for (const std::shared_ptr<State> &state : q)
                if (state->getName() == trim(matrix[i][0]))
                {
                    exitState = state;
                    break;
                }

            // Establecer symbol
            for (const std::shared_ptr<Symbol> &s : sigma)
                if (s->getName() == trim(matrix[0][j]))
                {
                    symbol = s;
                    break;
                }

            // Establecer arrival States
            for (const std::string &arrival : arrivalVector)
            {
                // buscamos si el estado ya existe
                std::shared_ptr<State> q_ptr = nullptr;
                for (const std::shared_ptr<State> &state : q)
                    if (state->getName() == arrival)
                    {
                        q_ptr = state;
                        break;
                    }

                if (q_ptr != nullptr)
                    arrivalStates.insert(q_ptr);
            }

            // hacer la transición
            Transition transition(exitState, symbol, arrivalStates);
            delta.insert(std::make_shared<Transition>(transition));
        }
}

void FiniteAutomaton::setClosure(std::shared_ptr<Transition> transition)
{
    std::shared_ptr<State> exitState = transition->getExitState();

    if (exitState->getIsClosureSet())
        return;
    exitState->setIsClosureSet(true);

    // Para cada estado de llegada con una transición epsilon
    for (const std::shared_ptr<State> &arrival : transition->getArrivalStates())
    {
        // Añadimos el estado de llegada a la clausura del estado de salida
        exitState->insertClosure(arrival);

        // Buscamos todas las transiciones cuyo estado de salida sea el estado de llegada actual
        for (const std::shared_ptr<Transition> &trans : delta) // Uso de una variable diferente "trans" para evitar confusión
            if (trans->getExitState() == arrival && trans->getSymbol()->isEpsilon())
            {
                // Llamada recursiva para calcular la clausura del estado de llegada
                setClosure(trans);
                break;
            }

        // Después de calcular la clausura del estado de llegada, añadimos todos los estados de su clausura
        for (const std::shared_ptr<State> &closure_state : arrival->getClosure())
            exitState->insertClosure(closure_state);
    }
}

// Impresiones
void FiniteAutomaton::printTuple()
{
    std::cout << "\nStates(q): ";
    for (std::shared_ptr<State> const &state : q)
        std::cout << state->getName() << " ";

    std::cout << "\nAlphabet(sigma): ";
    for (std::shared_ptr<Symbol> const &symbol : sigma)
        std::cout << symbol->getName() << " ";

    std::cout << "\nTransitions (delta): \n\t";
    for (std::shared_ptr<Transition> const &transition : delta)
    {

        std::cout << transition->getExitState()->getName() << "(" << transition->getSymbol()->getName() << ") -> { ";
        for (const std::shared_ptr<State> &delta : transition->getArrivalStates())
            std::cout << delta->getName() << " ";
        std::cout << "}\n\t";
    }

    std::cout << "\nStart state (q_0): " << q_0->getName();

    std::cout << "\nFinal states (F): ";
    for (std::shared_ptr<State> const &state : f)
        std::cout << state->getName() << " ";

    std::cout << std::endl;
}

void FiniteAutomaton::printMatrix()
{
    for (std::shared_ptr<Symbol> const &symbol : this->sigma)
        std::cout << "\t" << symbol->getName();

    std::cout << std::endl;

    for (const std::shared_ptr<State> &state : this->q)
    {
        if (state == q_0)
            std::cout << "-";

        if (this->f.find(state) != this->f.end())
            std::cout << "*";
        std::cout << state->getName() << "\t";
        for (std::shared_ptr<Symbol> const &symbol : this->sigma)
        {
            std::cout << "{ ";
            for (std::shared_ptr<Transition> const &transition : this->delta)
                if (transition->getExitState() == state && transition->getSymbol() == symbol)
                    for (const std::shared_ptr<State> &delta : transition->getArrivalStates())
                        std::cout << delta->getName() << " ";
            std::cout << "}\t";
        }
        std::cout << std::endl;
    }
}

void FiniteAutomaton::printStates()
{
    for (const std::shared_ptr<State> &state : q)
    {
        std::cout << "CLO(" << state->getName() << "): " << state->getName() << " ";
        for (const std::shared_ptr<State> &closure : state->getClosure())
            std::cout << closure->getName() << " ";
        std::cout << std::endl;
    }
}

// Solution
bool FiniteAutomaton::isChainValid(std::vector<std::string> chain)
{
    for (const std::string &letter : chain)
    {
        if (letter == "E")
            return false;

        bool isInSigma = false;
        for (const std::shared_ptr<Symbol> &symbol : sigma)
            if (symbol->getName() == letter)
                isInSigma = true;

        if (!isInSigma)
            return false;
    }

    return true;
}

bool FiniteAutomaton::testChain(std::vector<std::string> chain, bool final, std::shared_ptr<State> currentState)
{
    if (currentState == nullptr)
        currentState = q_0;

    if (final)
        return final;

    // Paso recursivo para las transiciones con epsilon
    if (!currentState->getClosure().empty())
        for (const std::shared_ptr<State> &epsilonState : currentState->getClosure())
            if (testChain(chain, final, epsilonState))
                return true;

    if (chain.empty())
        if (f.find(currentState) != f.end())
            return true;
        else
        {
            printStates();
            for (const std::shared_ptr<State> &closure : currentState->getClosure())
                if (f.find(closure) != f.end())
                    return true;

            return false;
        }

    // Paso recursivo para las transiciones directas
    for (const std::shared_ptr<Transition> &transition : delta)
        if (transition->getExitState() == currentState && transition->getSymbol()->getName() == chain[0])
            // Iterar sobre los estados de llegada (arrival States)
            for (const std::shared_ptr<State> &arrival : transition->getArrivalStates())
                if (testChain(std::vector<std::string>(chain.begin() + 1, chain.end()), final, arrival))
                    return true; // Si alguna transición acepta la cadena, retornar true

    return false;
}

// Conversiones
void FiniteAutomaton::nfae2nfa()
{
    // Seleccionamos las transiciones y estados que contienen a Epsilon
    std::set<std::shared_ptr<Transition>> delta_epsilon;
    for (const std::shared_ptr<Transition> &transition : delta)
        if (transition->getSymbol()->isEpsilon())
            delta_epsilon.insert(transition);

    // Se itera sobre las transiciones con epsilon
    for (const std::shared_ptr<Transition> &transition : delta_epsilon)
    {
        delta.erase(transition);

        // Se crea un nuevo estado, que es: q_i U CLO(q_i)
        std::shared_ptr<State> newState = closure2state(transition->getExitState());
        std::set<std::shared_ptr<State>> completeClosure = transition->getExitState()->getClosure();
        completeClosure.insert(transition->getExitState());

        // Iteramos sobre los simbolos
        for (const std::shared_ptr<Symbol> &symbol : sigma)
        {
            if (symbol->isEpsilon())
                continue;

            // Nuevo arrival States
            std::set<std::shared_ptr<State>> newArrivalStates;

            // Iteramos sobre los estados
            for (const std::shared_ptr<State> &closure : completeClosure)
            {
                // Buscamos si existe una transicion con existaState = closure & symbol = symbol
                for (const std::shared_ptr<Transition> &trans : delta)
                    if (trans->getExitState() == closure && trans->getSymbol() == symbol)
                    {
                        newArrivalStates.insert(trans->getArrivalStates().begin(), trans->getArrivalStates().end());
                        break;
                    }

                // Buscamos si algun estado pertenece en f
                if (f.find(closure) != f.end())
                    f.insert(newState);
            }

            // Creamos la nueva transición
            std::shared_ptr<Transition> transition_ptr = std::make_shared<Transition>(Transition(newState, symbol, newArrivalStates));
            delta.insert(transition_ptr);
        }

        // Intercambiamos los arrival states
        for (const std::shared_ptr<Transition> &trans : delta)
            for (const std::shared_ptr<State> &arrival : trans->getArrivalStates())
                if (arrival == transition->getExitState())
                {
                    trans->getArrivalStates().erase(arrival);
                    trans->getArrivalStates().insert(newState);
                }

        // Insertamos el estado en q
        q.insert(newState);
        if (q_0 == transition->getExitState())
            q_0 = newState;
        q.erase(transition->getExitState());
    }

    // Eliminar Epsilon
    for (std::set<std::shared_ptr<Symbol>>::iterator it = sigma.begin(); it != sigma.end();)
        if ((*it)->isEpsilon())
            it = sigma.erase(it);
        else
            ++it;

    type = "nfa";
}

void FiniteAutomaton::nfa2dfa()
{
    std::set<std::pair<std::shared_ptr<State>, std::set<std::shared_ptr<State>>>> visitedStates;
    std::set<std::pair<std::shared_ptr<State>, std::set<std::shared_ptr<State>>>> unvisitedStates;

    std::set<std::shared_ptr<Transition>> transitions;

    // Estado emptySet
    std::shared_ptr<State> emptySet = std::make_shared<State>(State("0"));
    std::set<std::shared_ptr<Transition>> emptyTransition;
    emptySet->setIsClosureSet(true);
    for (const std::shared_ptr<Symbol> &symbol : sigma)
        emptyTransition.insert(std::make_shared<Transition>(Transition(emptySet, symbol, {emptySet})));

    // Primer estado
    std::set<std::shared_ptr<State>> q_0_set;
    q_0_set.insert(q_0);
    unvisitedStates.insert(std::make_pair(q_0, q_0_set));

    // Mientras haya estados no visitados
    while (!unvisitedStates.empty())
    {
        std::pair<std::shared_ptr<State>, std::set<std::shared_ptr<State>>> currentState;
        currentState = *unvisitedStates.begin();
        unvisitedStates.erase(currentState);
        visitedStates.insert(currentState);

        // Iterar sobre los simbolos
        for (const std::shared_ptr<Symbol> &symbol : sigma)
        {
            // Acumular los arrivalStates
            std::pair<std::shared_ptr<State>, std::set<std::shared_ptr<State>>> arrivalState;

            // Iterar sobre los elementos del currentState
            for (const std::shared_ptr<State> &exitState : currentState.second)
                // Encontrar todos los arrivalStates de la transiciones y añadir a arrivalStates
                for (const std::shared_ptr<Transition> &trans : delta)
                    if (trans->getExitState() == exitState && trans->getSymbol() == symbol)
                        arrivalState.second.insert(trans->getArrivalStates().begin(), trans->getArrivalStates().end());

            std::shared_ptr<Transition> transition_ptr;
            if (arrivalState.second.empty())
                arrivalState.first = emptySet;
            else if (arrivalState.second.size() == 1)
                arrivalState.first = *arrivalState.second.begin();
            else
            {
                arrivalState.first = set2State(arrivalState.second);
                // Buscamos si el estado ya existe en los visited states por nombre
                for (const std::pair<std::shared_ptr<State>, std::set<std::shared_ptr<State>>> &visitedState : visitedStates)
                    if (visitedState.first->getName() == arrivalState.first->getName())
                        arrivalState.first = visitedState.first;
            }

            transition_ptr = std::make_shared<Transition>(Transition(currentState.first, symbol, {arrivalState.first}));

            bool isFinalState = false;
            for (const std::shared_ptr<State> &arrival : arrivalState.second)
                if (f.find(arrival) != f.end())
                    isFinalState = true;

            if (isFinalState)
                f.insert(arrivalState.first);

            if (visitedStates.find(arrivalState) == visitedStates.end())
                unvisitedStates.insert(arrivalState);

            transitions.insert(transition_ptr);
        }
    }

    std::set<std::shared_ptr<State>> new_q;
    for (const std::pair<std::shared_ptr<State>, std::set<std::shared_ptr<State>>> &state : visitedStates)
        new_q.insert(state.first);

    q = new_q;
    q.insert(emptySet);
    delta = transitions;
    delta.insert(emptyTransition.begin(), emptyTransition.end());

    type = "dfa";
}

std::shared_ptr<State> FiniteAutomaton::closure2state(std::shared_ptr<State> state)
{
    std::string name = state->getName();
    for (const std::shared_ptr<State> &closure : state->getClosure())
        name += closure->getName();

    std::shared_ptr<State> state_ptr = std::make_shared<State>(State(name));
    state_ptr->setIsClosureSet(true);

    return state_ptr;
}

std::shared_ptr<State> FiniteAutomaton::set2State(std::set<std::shared_ptr<State>> states)
{
    std::string name;
    for (const std::shared_ptr<State> &state : states)
        name += state->getName();
    std::shared_ptr<State> state_ptr = std::make_shared<State>(State(name));
    state_ptr->setIsClosureSet(true);

    return state_ptr;
}

std::vector<std::vector<std::string>> FiniteAutomaton::getInTuple()
{
    std::vector<std::vector<std::string>> a;
    return a;
}

std::vector<std::vector<std::string>> FiniteAutomaton::getInMatrix()
{
    std::vector<std::vector<std::string>> a;
    return a;
}

// Aditional functions
bool FiniteAutomaton::is_q_0(std::string &q)
{
    if (q[0] == '-')
    {
        q = q.substr(1, q.length() - 1);
        return true;
    }
    return false;
}

bool FiniteAutomaton::is_f(std::string &q)
{
    if (q[0] == '*')
    {
        q = q.substr(1, q.length() - 1);
        return true;
    }
    return false;
}

std::vector<std::string> FiniteAutomaton::splitString(std::string arrivalStates)
{
    std::vector<std::string> states;
    std::string stateSaved;

    for (const char &character : arrivalStates)
    {
        if (!character || character == ' ')
        {
            states.push_back(stateSaved);
            stateSaved = "";
            continue;
        }
        stateSaved += character;
    }

    states.push_back(stateSaved);
    return states;
}

std::string FiniteAutomaton::trim(std::string word)
{
    std::string result = "";
    for (const char &a : word)
    {
        if (a == ' ' || a == '-' || a == '*')
            continue;
        result += a;
    }
    return result;
}