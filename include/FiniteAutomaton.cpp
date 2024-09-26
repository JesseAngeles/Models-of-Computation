#include "FiniteAutomaton.h"

FiniteAutomaton::FiniteAutomaton(std::vector<std::vector<std::string>> &af)
{
    if (af[0][0] == "t")
        setFromTuple(af);
    else
        setFromMatrix(af);

    printTuple();
    printMatrix();
}

// Establecer los valores
void FiniteAutomaton::setFromTuple(std::vector<std::vector<std::string>> tuple)
{
    // Establecer q
    for (int j = 1; j < tuple[0].size(); j++)
        q.insert(trim(tuple[0][j]));

    // Establecer sigma
    for (int j = 0; j < tuple[1].size(); j++)
        sigma.insert(trim(tuple[1][j]));

    // Establecer delta
    for (int j = 0; j < tuple[2].size(); j++)
    {
        std::vector<std::string> data = splitString(tuple[2][j]);

        std::set<State>::iterator q_0_it = q.find(trim(data[0]));
        std::set<Symbol>::iterator sigma_it = sigma.find(trim(data[1]));
        std::set<State>::iterator q_it = q.find(trim(data[2]));

        if (q_0_it == q.end() || sigma_it == sigma.end() || q_it == q.end())
            return;
        
        Transition newTransition(*q_0_it, *sigma_it, {*q_it});

        for (const Transition &transition : delta)
            if (transition.getExitState() == newTransition.getExitState() && transition.getSymbol() == newTransition.getSymbol())
            {
                newTransition.insertArrivalState(transition.getArrivalStates());
                delta.erase(transition);
                break;
            }

        delta.insert(newTransition);
    }

    // Establecer q_0
    std::set<State>::iterator q_it = q.find(trim(tuple[3][0]));
    if (q_it != q.end())
        q_0 = *q_it;

    // Establecer f
    for (int i = 0; i < tuple[4].size(); i++)
    {
        std::set<State>::iterator q_it = q.find(trim(tuple[4][i]));
        if (q_it != q.end())
            f.insert(*q_it);
    }
}

void FiniteAutomaton::setFromMatrix(std::vector<std::vector<std::string>> matrix)
{
    // Estabelcer q, q_0 y f
    for (int i = 1; i < matrix.size(); i++)
    {
        std::string q_current = matrix[i][0];
        bool q_0_flag = false;
        bool f_flag = false;

        if (is_q_0(q_current) && !q_0)
            q_0_flag = true;

        if (is_f(q_current))
            f_flag = true;

        // Inserciones
        if (q_0_flag)
            q_0 = trim(q_current);
        if (f_flag)
            f.insert(trim(q_current));
        q.insert(trim(q_current));

        matrix[i][0] = q_current;
    }

    // Establecer sigma
    for (int j = 1; j < matrix[0].size(); j++)
        sigma.insert(trim(matrix[0][j]));

    // Establecer delta
    for (int i = 1; i < matrix.size(); i++)
        for (int j = 1; j < matrix[i].size(); j++)
        {
            if (matrix[i][j].empty())
                continue;

            std::set<State> arrivalStates;
            std::vector<std::string> arrivalVector = splitString(matrix[i][j]);
            for (const std::string &arrival : arrivalVector)
                arrivalStates.insert(trim(arrival));
            delta.insert(Transition(trim(matrix[i][0]), matrix[0][j], arrivalStates));
        }
}

void FiniteAutomaton::setClosures(State &state)
{
    std::set<State> arrivalStates;
    for (const Transition &transition : delta)
        if (transition.getSymbol().isEpsilon() && transition.getExitState() == state)
            arrivalStates.insert(transition.getArrivalStates().begin(), transition.getArrivalStates().end());

    for (const State &arrivalState : arrivalStates)
    {
        std::shared_ptr<State> state_ptr = std::make_shared<State>(arrivalState);
        state.insertClosure(state_ptr);
    }
}

// Impresiones
void FiniteAutomaton::printTuple()
{
    std::cout << "\nStates(q): ";
    for (auto const &state : q)
        std::cout << state.getName() << ". ";

    std::cout << "\nAlphabet(sigma): ";
    for (auto const &symbol : sigma)
        std::cout << symbol.getName() << ". ";

    std::cout << "\nTransitions (delta): \n\t";
    for (auto const &transition : delta)
    {
        std::cout << transition.getExitState().getName() << "(" << transition.getSymbol().getName() << ") -> { ";
        for (const auto &delta : transition.getArrivalStates())
            std::cout << delta.getName() << " ";
        std::cout << "}\n\t";
    }

    std::cout << "\nStart state (q_0): " << q_0.getName();

    std::cout << "\nFinal states (F): ";
    for (auto const &state : f)
        std::cout << state.getName() << ". ";

    std::cout << std::endl;
}

void FiniteAutomaton::printMatrix()
{
    for (auto const &symbol : this->sigma)
        std::cout << "\t" << symbol.getName();

    std::cout << std::endl;

    for (auto const &state : this->q)
    {
        if (state == this->q_0)
            std::cout << "-";
        if (this->f.find(state) != this->f.end())
            std::cout << "*";
        std::cout << state.getName() << "\t";
        for (auto const &symbol : this->sigma)
        {
            std::cout << "{ ";
            for (auto const &transition : this->delta)
                if (transition.getExitState() == state && transition.getSymbol() == symbol)
                    for (const auto &delta : transition.getArrivalStates())
                        std::cout << delta.getName() << " ";
            std::cout << "}\t";
        }
        std::cout << std::endl;
    }
}

// TODO Solution
bool FiniteAutomaton::findEpsilonCicle()
{
    // Obtenemos todos los delta que contengan epsilon
    std::set<Transition> deltaEpsilon;
    for (const Transition &transition : delta)
        if (transition.getSymbol().isEpsilon())
            deltaEpsilon.insert(transition);

    State currentState;
    for (const Transition &transition : deltaEpsilon)
    {
    }

    return false;
}

// Conversiones
void FiniteAutomaton::nfa2dfa()
{
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
        if (a == ' ')
            continue;
        result += a;
    }
    return result;
}