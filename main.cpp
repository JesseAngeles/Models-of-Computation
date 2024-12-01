#include <iostream>

#include "PDA/PushdownAutomaton.hpp"
#include "CFG/ContextFreeGrammar.hpp"

using namespace std;

int main()
{
    std::shared_ptr<NonTerminalSymbol> S_sym = std::make_shared<NonTerminalSymbol>("S");

    std::shared_ptr<TerminalSymbol> a_sym = std::make_shared<TerminalSymbol>("a");
    std::shared_ptr<TerminalSymbol> b_sym = std::make_shared<TerminalSymbol>("b");

    std::vector<std::shared_ptr<Symbol>> prod1_res;
    prod1_res.push_back(a_sym);
    prod1_res.push_back(S_sym);
    prod1_res.push_back(b_sym);

    std::vector<std::shared_ptr<Symbol>> prod2_res;

    std::shared_ptr<ProductionRule> prod1 = std::make_shared<ProductionRule>(S_sym, prod1_res);
    std::shared_ptr<ProductionRule> prod2 = std::make_shared<ProductionRule>(S_sym, prod2_res);

    std::set<shared_ptr<NonTerminalSymbol>> non_terminal_symbols;
    non_terminal_symbols.insert(S_sym);

    std::set<shared_ptr<TerminalSymbol>> terminal_symbols;
    terminal_symbols.insert(a_sym);
    terminal_symbols.insert(b_sym);

    std::set<shared_ptr<ProductionRule>> prods;
    prods.insert(prod1);
    prods.insert(prod2);

    ContextFreeGrammar cfg(non_terminal_symbols, terminal_symbols, prods, S_sym);
    cfg.display();

    ContextFreeGrammar cfg2("./resources/CFG/1_rules.csv");
    std::cout << "\n\n";
    cfg2.display();
}
