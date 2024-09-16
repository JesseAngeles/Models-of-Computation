#include "../include/nfa.h"

NFA::NFA(const vector<vector<string>> &matrix)
{
    if (matrix[0][0] == "m")
        setFromMatrix(matrix);
    else
        setFromTuple(matrix);

    printMatrix();
    printTuple();
}

void NFA::setFromMatrix(vector<vector<string>> matrix)
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
    {
        for (int j = 1; j < matrix[i].size(); j++)
        {
            this->delta.insert({trim(matrix[i][0]),
                                trim(matrix[0][j])[0],
                                {getDelta(matrix[i][j])}});
        }
    }
}

void NFA::setFromTuple(vector<vector<string>> matrix)
{
    matrix[0].erase(matrix[0].begin());
    // Establece q
    for (auto const &state : matrix[0])
        this->q.insert(trim(state));

    // Establece sigma
    for (auto const &symbol : matrix[1])
        this->sigma.insert(trim(symbol)[0]);

    //! Establecer delta
    for (const auto &transition : matrix[2])
    {
        tuple<string, char, set<string>> data = splitString(transition);

        if (this->q.find(get<0>(data)) != this->q.end() &&
            this->sigma.find(get<1>(data)) != this->sigma.end())
        {
            bool flag = true;
            for (const auto &delta : get<2>(data))
                if (this->q.find(delta) == this->q.end())
                    flag = false;

            if (flag)
                this->delta.insert(data);
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

void NFA::printMatrix()
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
                {
                    cout << "{";
                    for (const auto &delta : get<2>(transition))
                        cout << delta << " ";
                    cout << "}";
                }
        cout << endl;
    }
}

void NFA::printTuple()
{
    cout << "\nStates(q): ";
    for (auto const &state : this->q)
        cout << state << ". ";

    cout << "\nAlphabet(sigma): ";
    for (auto const &symbol : this->sigma)
        cout << symbol << ". ";

    cout << "\nTransitions (delta): \n\t";
    for (auto const &transition : this->delta)
    {
        cout << get<0>(transition) << "(" << get<1>(transition) << ") -> {";
        for (const auto &delta : get<2>(transition))
            cout << delta << " ";
        cout << "}\n\t";
    }
    cout << "\nStart state (q_0): " << this->q_0;

    cout << "\nFinal states (F): ";
    for (auto const &state : this->f)
        cout << state << ". ";

    cout << endl;
}

bool NFA::chainValid(string word)
{
    return false;
}

bool NFA::testChain(string word)
{
    return true;
}

set<string> NFA::getDelta(const string& data) {
    set<string> result;
    stringstream ss(data);
    string word;

    while (ss >> word) {
        result.insert(trim(word));
    }

    return result;
}

tuple<string, char, set<string>> NFA::splitString(string transition)
{
    vector<string> strings;
    string word;
    for (const auto &c : transition)
        if ((c == ' ' || c == '{' || c == '}') && !word.empty())
        {
            strings.push_back(trim(word));
            word.clear();
        }
        else
            word += c;

    if (!word.empty())
        strings.push_back(word);

    set<string> delta;
    for (int i = 2; i < strings.size(); i++)
        delta.insert(strings[i]);

    return make_tuple(strings[0], strings[1][0], delta);
}