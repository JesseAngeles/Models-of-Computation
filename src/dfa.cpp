#include "../include/dfa.h"

DFA::DFA(const vector<vector<string>> &matrix)
{
    if (matrix[0][0] == "m")
        setFromMatrix(matrix);
    else
        setFromTuple(matrix);

    printMatrix();
    printTuple();
}

void DFA::setFromMatrix(vector<vector<string>> matrix)
{
    // Establecer q, q_0, f
    for (int i = 1; i < matrix.size(); i++)
    {
        string word = matrix[i][0];
        string state = trim(word);

        if (word[0] == '-')
        {
            this->q_0 = state;
            word = string(word.begin() + 1, word.end());
        }
        if (word[0] == '*')
            this->f.insert(state);
        this->q.insert(state);
    }

    // Establacer sigma
    for (auto const &symbol : matrix[0])
        if (trim(symbol) != "")
            this->sigma.insert(trim(symbol)[0]);

    // Establecer delta
    for (int i = 1; i < matrix.size(); i++)
        for (int j = 1; j < matrix[i].size(); j++)
            this->delta.insert({trim(matrix[i][0]), trim(matrix[0][j])[0], trim(matrix[j][i])});
}

void DFA::setFromTuple(vector<vector<string>> matrix)
{
    // Establece q
    matrix[0].erase(matrix[0].begin());
    for (auto const &state : matrix[0])
        this->q.insert(trim(state));

    // Establece sigma
    for (auto const &symbol : matrix[1])
        this->sigma.insert(trim(symbol)[0]);

    // Establece delta
    for (auto const &transition : matrix[2])
    {
        tuple<string, string, string> data = splitString(transition);
        if (this->q.find(get<0>(data)) != this->q.end() && this->sigma.find(get<1>(data)[0]) != this->sigma.end() && this->q.find(get<2>(data)) != this->q.end())
        {
            this->delta.insert({get<0>(data), get<1>(data)[0], get<2>(data)});
        }
    }

    // Establece q_0
    if (this->q.find(trim(matrix[3][0])) != this->q.end())
        this->q_0 = trim(matrix[3][0]);

    // Establece f
    for (auto const &state : matrix[4])
        if (this->q.find(trim(state)) != this->q.end())
            this->f.insert(trim(state));
}

void DFA::printMatrix()
{
    for (auto const &symbol : this->sigma)
        cout << "\t" << symbol;

    cout << endl;

    for (auto const &state : this->q)
    {
        if (state == this->q_0)
            cout << "-";
        if (this->f.find(state) != this->f.end())
            cout << "*";
        cout << state << "\t";
        for (auto const &symbol : this->sigma)
            for (auto const &transition : this->delta)
                if (get<0>(transition) == state && get<1>(transition) == symbol)
                    cout << get<2>(transition) << "\t";

        cout << endl;
    }
}

void DFA::printTuple()
{
    cout << "\nStates(q): ";
    for (auto const &state : this->q)
        cout << state << ". ";

    cout << "\nAlphabet(sigma): ";
    for (auto const &symbol : this->sigma)
        cout << symbol << ". ";

    cout << "\nTransitions (delta): \n\t";
    for (auto const &transition : this->delta)
        cout << get<0>(transition) << "(" << get<1>(transition)
             << ") -> " << get<2>(transition) << "\n\t";

    cout << "\nStart state (q_0): " << this->q_0;

    cout << "\nFinal states (F): ";
    for (auto const &state : this->f)
        cout << state << ". ";

    cout << endl;
}

bool DFA::chainValid(string word)
{
    for (auto const &symbol : word)
        if (this->sigma.find(symbol) == this->sigma.end())
            return false;

    return true;
}

bool DFA::testChain(string word)
{
    string currentState = this->q_0;
    for (auto const &symbol : word)
    {
        for (auto const &transition : this->delta)
        {
            if (get<0>(transition) == currentState && get<1>(transition) == symbol)
            {
                currentState = get<2>(transition);
                break;
            }
        }
    }

    if (this->f.find(currentState) != this->f.end())
        return true;
    return false;
}

tuple<string, string, string> DFA::splitString(const string &str)
{
    vector<string> subStrings;
    istringstream iss(str);
    string word;

    // Extraer cada palabra del string separado por espacios
    while (iss >> word)
    {
        subStrings.push_back(word);
    }

    return {trim(subStrings[0]), trim(subStrings[1]), trim(subStrings[2])};
}

void DFA::nfa2dfa(NFA nfa)
{
    set<tuple<string, char, set<string>>> nfa_delta = nfa.get_delta();

    this->sigma = nfa.get_sigma();
    this->q_0 = nfa.get_q_0();
    this->q.insert({q_0});

    set<set<string>> visitedStates;
    set<set<string>> unvisitedStates;

    set<string> initialState;
    initialState.insert(q_0);
    unvisitedStates.insert(initialState);

    set<tuple<set<string>, char, set<string>>> multiTransitions;

    string emptyState = "Q";
    set<string> empty;
    empty.insert(emptyState);
    this->q.insert(emptyState);
    for (auto const &symbol : this->sigma)
        multiTransitions.insert(make_tuple(empty, symbol, empty));

    do
    {
        cout << "Unvisited states: " << unvisitedStates.size() << endl;
        set<string> currentState = *unvisitedStates.begin();
        unvisitedStates.erase(unvisitedStates.begin());
        visitedStates.insert(currentState);

        for (auto const &state : currentState)
        {
            currentState.erase(state);
            currentState.insert(trim(state));
        }

        for (char symbol : this->sigma) // Se iteran los simbolos
        {
            set<string> nextStates;

            set<tuple<string, char, set<string>>> transWithExitAndSymbol;
            for (auto const &trans : nfa_delta) // Se iteran las transiciones
            {
                if (currentState.find(get<0>(trans)) != currentState.end() && symbol == get<1>(trans))
                    transWithExitAndSymbol.insert(trans);
            }

            // Se crea la multitransition
            set<string> exitStates;
            set<string> arrivalStates;
            for (auto const &trans : transWithExitAndSymbol)
            {
                cout << setToString(get<2>(trans)) << endl;
                exitStates.insert(get<0>(trans));
                arrivalStates.insert(get<2>(trans).begin(), get<2>(trans).end());

                // Se añaden a invistedStates
                if (visitedStates.find(get<2>(trans)) == visitedStates.end())
                    unvisitedStates.insert(get<2>(trans));
            }
            if (arrivalStates.empty())
                arrivalStates.insert(emptyState);

            multiTransitions.insert(make_tuple(exitStates, symbol, arrivalStates));
        }
    } while (!unvisitedStates.empty());

    for (auto const &trans : multiTransitions)
    {
        string exitState = setToString(get<0>(trans));
        string arrivalState = setToString(get<2>(trans));
        this->delta.insert(make_tuple(exitState, get<1>(trans), arrivalState));
        this->q.insert(exitState);
        this->q.insert(arrivalState);

        // Establecer salidas
        if (get<2>(trans).find("q2") != get<2>(trans).end())
            this->f.insert(arrivalState);
    }

    this->q.erase(this->q.find(""));
    
    for (auto it = delta.begin(); it != delta.end(); ++it) {
        if (get<0>(*it) == "") {
            delta.erase(it);
            break;  
        }
    }
}

string DFA::setToString(set<string> info)
{
    string name;
    for (auto const &str : info)
        name += trim(str);
    return name;
}
