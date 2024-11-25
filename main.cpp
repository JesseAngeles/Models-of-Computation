#include <iostream>

#include "PDA/PushdownAutomaton.hpp"

using namespace std;

int main()
{
    State q0("q0");
    State q1("q1");
    State q2("q2");

    InputSymbol a("a");
    InputSymbol b("b");

    StackSymbol z0;
    StackSymbol s("1");

    stack<StackSymbol> s1;
    s1.push(z0);
    s1.push(s);

    stack<StackSymbol> s2;
    s2.push(s);
    s2.push(s);

    Transition t1(q0, q0, a, z0, s1);
    Transition t2(q0, q0, a, s, s2);
    
    Transition t3(q0, q1, nullopt,z0, z0);
    Transition t4(q0, q1, nullopt,s, s);

    Transition t5(q1, q1, b, s, nullopt);

    Transition t6(q1, q2, nullopt,z0, z0);

    set<State> states = {q0, q1, q2};
    set<InputSymbol> inputAlphabet = {a, b};
    set<StackSymbol> stackSymbol = {z0, s};
    set<Transition> transitions = {t1, t2, t3, t4, t5, t6};

    PushdownAutomaton pda(states, inputAlphabet, stackSymbol, transitions, q0, z0, {q2});

    pda.display();

    cout << endl;

    PushdownAutomaton pda2("resources/PDA/test_1.csv");
    pda2.display();
}
