#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/FiniteAutomaton.h"

using namespace std;

int Menu();
vector<vector<string>> readCSV(int &, string);
std::vector<std::string> chainSplit(std::string word);
void testChain(std::shared_ptr<FiniteAutomaton>);

int main()
{
    int option, error = 0;
    std::shared_ptr<FiniteAutomaton> automaton = nullptr;

    do
    {
        option = Menu();
        switch (option)
        {
        case 1:
            automaton = std::make_shared<FiniteAutomaton>(readCSV(error, "nfa"));
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
                automaton->printStates();
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

int Menu()
{
    int option;
    do
    {
        cout << "0) Exit" << endl;
        cout << "1) Set Automaton" << endl;
        cout << "2) Test Chain" << endl;
        cout << "3) Print in Matrix" << endl;
        cout << "4) Print in 5 Tuple" << endl;
        cout << "5) Print closures" << endl;
        cin >> option;
    } while (option < 0 && option > 5);
    return option;
}

vector<vector<string>> readCSV(int &error, string path)
{
    string filename;
    cout << "Filename number: ";
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

    if(chain == "E")
        chain = "";

    vector<string> chains = chainSplit(chain);
    if (automaton->isChainValid(chains))
        if (automaton->testChain(chains))
            cout << "chain accepted" << endl;
        else
            cout << "chain rejected" << endl;
    else
        cout << "chain invalid" << endl;
}