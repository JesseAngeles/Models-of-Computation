#include <iostream>

#include "PDA/PushdownAutomaton.hpp"

using namespace std;

int main()
{
    std::shared_ptr<State> q0 = std::make_shared<State>("q0");
    std::shared_ptr<State> q1 = std::make_shared<State>("q1");
    std::shared_ptr<State> q2 = std::make_shared<State>("q2");

    std::shared_ptr<InputSymbol> a = std::make_shared<InputSymbol>("a");
    std::shared_ptr<InputSymbol> b = std::make_shared<InputSymbol>("b");

    std::shared_ptr<StackSymbol> z0 = std::make_shared<StackSymbol>("z0");
    std::shared_ptr<StackSymbol> s = std::make_shared<StackSymbol>("1");

    stack<std::shared_ptr<StackSymbol>> s1;
    s1.push(z0);
    s1.push(s);

    stack<std::shared_ptr<StackSymbol>> s2;
    s2.push(s);
    s2.push(s);

    std::shared_ptr<Transition> t1 = std::make_shared<Transition>(q0, q0, a, z0, s1);
    std::shared_ptr<Transition> t2 = std::make_shared<Transition>(q0, q0, a, s, s2);

    std::shared_ptr<Transition> t3 = std::make_shared<Transition>(q0, q1, nullopt, z0, z0);
    std::shared_ptr<Transition> t4 = std::make_shared<Transition>(q0, q1, nullopt, s, s);

    std::shared_ptr<Transition> t5 = std::make_shared<Transition>(q1, q1, b, s, nullopt);

    std::shared_ptr<Transition> t6 = std::make_shared<Transition>(q1, q2, nullopt, z0, z0);

    set<std::shared_ptr<State>> states = {q0, q1, q2};
    set<std::shared_ptr<InputSymbol>> inputAlphabet = {a, b};
    set<std::shared_ptr<StackSymbol>> stackSymbol = {z0, s};
    set<std::shared_ptr<Transition>> transitions = {t1, t2, t3, t4, t5, t6};
    set<std::shared_ptr<State>> final_states;
    final_states.insert(q2);

    PushdownAutomaton pda2(states, inputAlphabet, stackSymbol, transitions, q0, z0, final_states);

    // pda.display();

    PushdownAutomaton pda("resources/PDA/test_1.csv");
    pda2.display();


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
