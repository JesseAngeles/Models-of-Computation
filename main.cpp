#include <iostream>

#include "PDA/PushdownAutomaton.hpp"

using namespace std;

int main()
{
    PushdownAutomaton pda("resources/PDA/1_empty_stack2.csv");
    pda.display();

    cout << endl;
    string test;
    cout << "test: ";
    cin >> test;

    if(test == "E")
        test = "";

    bool res = pda.testChain(test);
    std::cout << "\nres: " << res << "\n";

    cout << endl;
}
