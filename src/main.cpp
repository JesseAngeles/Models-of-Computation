#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/FiniteAutomaton.h"

using namespace std;

void Menu();
vector<vector<string>> readCSV(int &, string);

// void test(DFA &dfa);
// void test(NFA &nfa);

int main()
{
    int error;
    vector<vector<string>> data = readCSV(error, "nfa");

    FiniteAutomaton fa = FiniteAutomaton(data);

    bool flag = false;

    fa.printStates();

    return 0;
}

// int min()
// {
//     int option = 0;
//     int currentAutomaton = 0;
//     DFA dfa;
//     NFA nfa;

//     int error = 0;

//     do
//     {
//         error = 0;
//         Menu();
//         if (currentAutomaton == -1)
//             cout << "6) NFA to DFA\n";
//         cin >> option;
//         switch (option)
//         {
//         case 1:
//         {
//             vector<vector<string>> matrix = readCSV(error, "dfa");
//             if (error != 0)
//                 continue;
//             dfa = DFA(matrix);
//             currentAutomaton = 1;
//         }
//         break;
//         case 2:
//         {
//             vector<vector<string>> matrix = readCSV(error, "nfa");
//             if (error != 0)
//                 continue;
//             nfa = NFA(matrix);
//             currentAutomaton = -1;
//         }
//         break;
//         case 3:
//             if (currentAutomaton == 1)
//                 test(dfa);
//             else if (currentAutomaton == -1)
//                 test(nfa);
//             else
//                 cout << "\nSet automaton before\n";
//             break;
//         case 4:
//             if (currentAutomaton == 1)
//                 dfa.printTuple();
//             else if (currentAutomaton == -1)
//                 nfa.printTuple();
//             else
//                 cout << "\nSet automaton before\n";
//             break;
//         case 5:
//             if (currentAutomaton == 1)
//                 dfa.printMatrix();
//             else if (currentAutomaton == -1)
//                 nfa.printMatrix();
//             else
//                 cout << "\nSet automaton before\n";
//             break;
//         case 6:
//             if (currentAutomaton == -1)
//             {
//                 currentAutomaton = 1;
//                 dfa.nfa2dfa(nfa);
//             }
//         }

//     } while (option);

//     return 0;
// }

void Menu()
{
    cout << "0) Exit" << endl;
    cout << "1) Set dfa" << endl;
    cout << "2) Set nfa" << endl;
    cout << "3) Test chain" << endl;
    cout << "4) Print in 5 tuple" << endl;
    cout << "5) Print in matrix" << endl;
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

// void test(DFA &dfa)
// {
//     string word;
//     cout << "Word ('.' for Epsilon): ";
//     cin >> word;

//     if (word == ".")
//         word.clear();

//     bool valid, status;

//     if (dfa.chainValid(word))
//     {
//         status = dfa.testChain(word);
//         if (status)
//             cout << "\nChain accepted\n";
//         else
//             cout << "\nChain rejected\n";
//     }
//     else
//         cout << "\nInvalid chain\n";
// }

// void test(NFA &nfa)
// {
//     string word;
//     cout << "Word: ";
//     cin >> word;

//     bool valid, status;

//     if (nfa.chainValid(word))
//     {
//         status = nfa.testChain(word);
//         if (status)
//             cout << "\nChain accepted\n";
//         else
//             cout << "\nChain rejected\n";
//     }
//     else
//         cout << "\nInvalid chain\n";
// }