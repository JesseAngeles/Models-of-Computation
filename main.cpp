#include <iostream>

#include "PDA/PushdownAutomaton.hpp"

using namespace std;

int main()
{
    PushdownAutomaton pda("resources/PDA/1_final_states.csv");
    pda.display();

    cout << endl;
    string test;
    cout << "test: ";
    cin >> test;

    if (test == "E")
        test = "";

    std::cout << "\nIs empty set: " << pda.isByEmptySet() << "\n";

    bool res = pda.testChain(test);
    std::cout << "\nres: " << res << "\n";

    pda.final_state2empty_set();

    pda.display();


    cout << endl;
    test;
    cout << "test: ";
    cin >> test;

    if(test == "E")
        test = "";

    std::cout << "\nIs empty set: " << pda.isByEmptySet() << "\n";

    res = pda.testChain(test);
    std::cout << "\nres: " << res << "\n";

    cout << endl;
}
