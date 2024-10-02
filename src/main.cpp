#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/FiniteAutomaton.h"

#define divisor "*************************"

using namespace std;

vector<vector<string>> readCSV(int &, string);
std::vector<std::string> chainSplit(std::string word);
void testChain(std::shared_ptr<FiniteAutomaton>);

int main()
{
    int option, error = 0;
    std::shared_ptr<FiniteAutomaton> automaton = nullptr;

    do
    {
        cout << divisor << endl;
        cout << "0) Exit" << endl;
        cout << "1) Set Automaton" << endl;
        if (automaton != nullptr)
        {
            cout << "2) Test Chain" << endl;
            cout << "3) Print in Matrix" << endl;
            cout << "4) Print in 5 Tuple" << endl;

            if (automaton->getType() == "nfae")
            {
                cout << "5) NFA-E to NFA" << endl;
                cout << "6) NFA-E to DFA" << endl;
            }
            else if (automaton->getType() == "nfa")
                cout << "7) NFA to DFA" << endl;
        }

        cin >> option;
        switch (option)
        {
        case 0:
            "bye";
            break;
        case 1:
        {
            automaton = std::make_shared<FiniteAutomaton>(readCSV(error, "nfa"));
            automaton->printStates();
        }
        break;
        case 2:
            if (automaton != nullptr)
                testChain(automaton);
            break;
        case 3:
            if (automaton != nullptr)
                automaton->printMatrix();
            break;
        case 4:
            if (automaton != nullptr)
                automaton->printTuple();
            break;
        case 5:
            if (automaton != nullptr)
                if (automaton->getType() == "nfae")
                    automaton->nfae2nfa();
                else if (automaton->getType() == "nfa")
                    automaton->nfa2dfa();
            break;
        case 6:
            if (automaton != nullptr && automaton->getType() == "nfae")
            {
                automaton->nfae2nfa();
                automaton->nfa2dfa();
            }
            break;
        }
    } while (option);

    vector<vector<string>> data = readCSV(error, "nfa");

    FiniteAutomaton fa = FiniteAutomaton(data);

    fa.printMatrix();

    std::cout << fa.testChain(chainSplit("01")) << std::endl;

    return 0;
}

std::vector<std::string> chainSplit(std::string word)
{
    std::vector<std::string> result;

    for (char c : word)
        result.push_back(std::string(1, c));
    return result;
}

vector<vector<string>> readCSV(int &error, string path)
{
    string filename;
    cout << "Filename: ";
    cin >> filename;

    filename = "resources/" + path + filename + ".csv";

    ifstream file(filename);
    if (!file.is_open())
    {
        error = -1;
        vector<vector<string>> s;
        cout << "Can´t find " << path << endl;
        return s;
    }

    string line, word;
    vector<vector<string>> matrix;

    while (getline(file, line))
    {
        vector<string> row;
        stringstream ss(line);

        while (getline(ss, word, ','))
            row.push_back(word);

        matrix.push_back(row);
    }

    file.close();

    return matrix;
}

void testChain(std::shared_ptr<FiniteAutomaton> automaton)
{
    string chain = "";

    cout << "E for empty chain: ";
    cin >> chain;

    if (chain == "E")
        chain = "";

    vector<string> chains = chainSplit(chain);
    if (automaton->isChainValid(chains))
        if (automaton->testChain(chains))
            cout << "-- CHAIN ACCEPTED --" << endl;
        else
            cout << "-- CHAIN REJECTED --" << endl;
    else
        cout << "** CHAIN INVALID **" << endl;
}