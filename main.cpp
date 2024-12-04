#include <iostream>

#include "PDA/PushdownAutomaton.hpp"
#include "CFG/ContextFreeGrammar.hpp"

using namespace std;

int main()
{
    PushdownAutomaton pda("./resources/PDA/2_empty_stack.csv");
    pda.display();
    cout << "\n";
    
    ContextFreeGrammar cfg = pda.toCFG();
    cfg.display();
    cfg.clean();
    cfg.display();
}
