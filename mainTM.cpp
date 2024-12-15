#include <iostream>
#include <memory>
#include <string>

#include "TM/TuringMachine.hpp"

using namespace std;

vector<string> stringToVector(string &);

int main()
{
    // Numero par de ceros
    TuringMachine tm("resources/TM/pair.csv");
    tm.display();

    string chain;
    cout << "Chain: ";
    cin >> chain;

    vector<string> a = stringToVector(chain);

    auto b = tm.createChain(a);

    if (tm.testChain(b))
        cout << "true";
    else
        cout << "false";

    return 0;
}

vector<string> stringToVector(string &chain)
{
    vector<string> new_chain;

    for (auto c : chain)
    {
        string a = "";
        a+= c;
        new_chain.push_back(a);
    }

    return new_chain;
}