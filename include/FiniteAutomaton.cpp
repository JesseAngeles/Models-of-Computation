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
        sigma.insert(trim(tuple[1][j])[0]);

    // Establecer delta

    std::set<std::tuple<std::string, char, std::set<std::string>>> transitions;
    for (int j = 0; j < tuple[2].size(); j++)
    {
        std::vector<std::string> newTransition = splitString(tuple[2][j]);
        printVector(newTransition);
        bool transitionExists = false;
        for (auto &transition : transitions)
        {
            if (std::get<0>(transition) == newTransition[0] && std::get<1>(transition) == newTransition[1][0])
            {
                transitions.erase(transition);
                std::tuple<std::string, char, std::set<std::string>> trans = transition;
                std::get<2>(trans).insert(newTransition[2]);
                transitions.insert(trans);
                transitionExists = true;
                break;
            }
        }

        if (!transitionExists)
        {
            std::set<std::string> states(newTransition.begin() + 2, newTransition.end());
            transitions.insert(std::make_tuple(newTransition[0], newTransition[1][0], states));
        }
    }
    delta = transitions;

    // Establecer q_0
    q_0 = tuple[3][0];

    // Establecer f
    for (int i = 0; i < tuple[4].size(); i++)
        f.insert(tuple[4][i]);
}

void FiniteAutomaton::setFromMatrix(std::vector<std::vector<std::string>> matrix)
{
    // Estabelcer q, q_0 y f
    for (int i = 1; i < matrix.size(); i++)
    {
        std::string q_current = matrix[i][0];
        bool q_0_flag = false;
        bool f_flag = false;

        if (is_q_0(q_current) && q_0.empty())
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
        sigma.insert(trim(matrix[0][j])[0]);

    // Establecer delta
    for (int i = 1; i < matrix.size(); i++)
    {
        for (int j = 1; j < matrix[i].size(); j++)
        {
            if(matrix[i][j].empty())
                continue; 
            std::vector<std::string> arrivalVector = splitString(matrix[i][j]);
            std::set<std::string> arrivalSet;
            for (const std::string &arrival : arrivalVector)
                arrivalSet.insert(trim(arrival));
            delta.insert(std::make_tuple(trim(matrix[i][0]), trim(matrix[0][j])[0], arrivalSet));
        }
    }
}

// Impresiones
void FiniteAutomaton::printTuple()
{
    std::cout << "\nStates(q): ";
    for (auto const &state : q)
        std::cout << state << ". ";

    std::cout << "\nAlphabet(sigma): ";
    for (auto const &symbol : sigma)
        std::cout << symbol << ". ";

    std::cout << "\nTransitions (delta): \n\t";
    for (auto const &transition : delta)
    {
        std::cout << std::get<0>(transition) << "(" << std::get<1>(transition) << ") -> { ";
        for (const auto &delta : std::get<2>(transition))
            std::cout << delta << " ";
        std::cout << "}\n\t";
    }

    std::cout << "\nStart state (q_0): " << q_0;

    std::cout << "\nFinal states (F): ";
    for (auto const &state : f)
        std::cout << state << ". ";

    std::cout << std::endl;
}

void FiniteAutomaton::printMatrix()
{
    for (auto const &symbol : this->sigma)
        std::cout << "\t" << symbol;

    std::cout << std::endl;

    for (auto const &state : this->q)
    {
        if (state == this->q_0)
            std::cout << "-";
        if (this->f.find(state) != this->f.end())
            std::cout << "*";
        std::cout << state << "\t";
        for (auto const &symbol : this->sigma)
        {
            std::cout << "{ ";
            for (auto const &transition : this->delta)
                if (std::get<0>(transition) == state && std::get<1>(transition) == symbol)
                    for (const auto &delta : std::get<2>(transition))
                        std::cout << delta << " ";
            std::cout << "}\t";
        }
        std::cout << std::endl;
    }
}

void FiniteAutomaton::printVector(std::vector<std::string> stringVector)
{
    for (const std::string &item : stringVector)
        std::cout << item << "  ";
    std::cout << std::endl;
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