#include <iostream>

#include "PDA/PushdownAutomaton.hpp"
#include "CFG/ContextFreeGrammar.hpp"

using namespace std;

int main()
{
    ContextFreeGrammar cfg("./resources/CFG/2_rules.csv");
    cfg.display();
    cfg.clean();
    cfg.display();
}
