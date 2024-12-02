#ifndef CONTEXT_FREE_GRAMMAR_H
#define CONTEXT_FREE_GRAMMAR_H

#include <set>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>

#include "CFG/NonTerminalSymbol.hpp"
#include "TerminalSymbol.hpp"
#include "ProductionRule.hpp"

class ContextFreeGrammar
{
private:
    // Attributes
    std::set<std::shared_ptr<NonTerminalSymbol>> non_terminal_symbols;
    std::set<std::shared_ptr<TerminalSymbol>> terminal_symbols;
    std::set<std::shared_ptr<ProductionRule>> production_rules;
    std::shared_ptr<NonTerminalSymbol> start_symbol;

    // Functions
    std::vector<std::string> split(const std::string &line, char delimiter);
    bool isChainValid(const std::string &chain) const;
    std::string recursiveTest(const std::string &objective_chain,
                              std::string current_chain,
                              std::shared_ptr<NonTerminalSymbol> current_symbol);

public:
    // Constructor
    ContextFreeGrammar(
        const std::set<std::shared_ptr<NonTerminalSymbol>> &non_terminal_symbols,
        const std::set<std::shared_ptr<TerminalSymbol>> &terminal_symbols,
        const std::set<std::shared_ptr<ProductionRule>> &production_rules,
        const std::shared_ptr<NonTerminalSymbol> &start_symbol)
        : non_terminal_symbols(non_terminal_symbols),
          terminal_symbols(terminal_symbols),
          production_rules(production_rules),
          start_symbol(start_symbol)
    {
    }

    ContextFreeGrammar(const std::string &file_path);

    // Functions
    bool testChain(std::string);

    // Getters
    const std::set<std::shared_ptr<NonTerminalSymbol>> getNonTerminalSymbols() const { return non_terminal_symbols; }
    const std::set<std::shared_ptr<TerminalSymbol>> getTerminalSymbols() const { return terminal_symbols; }
    const std::set<std::shared_ptr<ProductionRule>> getProductionRules() const { return production_rules; }
    const std::shared_ptr<NonTerminalSymbol> getStartSymbol() const { return start_symbol; }

    // Setters
    void setNonTerminalSymbols(const std::set<std::shared_ptr<NonTerminalSymbol>> &non_terminal_symbols) { this->non_terminal_symbols = non_terminal_symbols; }
    void setTerminalSymbols(const std::set<std::shared_ptr<TerminalSymbol>> &terminal_symbols) { this->terminal_symbols = terminal_symbols; }
    void setProductionRules(const std::set<std::shared_ptr<ProductionRule>> &production_rules) { this->production_rules = production_rules; }
    void setStartSymbol(const std::shared_ptr<NonTerminalSymbol> &start_symbol) { this->start_symbol = start_symbol; }

    // Display
    void display() const;
};

#endif // CONTEXT_FREE_GRAMMAR_H