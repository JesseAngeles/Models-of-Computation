#ifndef PRODUCTION_RULE_H
#define PRODUCTION_RULE_H

#include <vector>
#include <memory>

#include "NonTerminalSymbol.hpp"
#include "TerminalSymbol.hpp"

class ProductionRule
{
private:
    // Attributes
    std::shared_ptr<NonTerminalSymbol> start_symbol;
    std::vector<std::shared_ptr<Symbol>> production;

public:
    // Constructor
    ProductionRule(
        const std::shared_ptr<NonTerminalSymbol> &start_symbol,
        const std::vector<std::shared_ptr<Symbol>> &production)
        : start_symbol(start_symbol), production(production) {}

    // Overloading
    bool operator<(const ProductionRule &other) const;
    
    // Getters
    const std::shared_ptr<NonTerminalSymbol> getStartSymbol() const { return start_symbol; }
    const std::vector<std::shared_ptr<Symbol>> getProduction() const { return production; }

    // Setters
    void setStartSymbol(const std::shared_ptr<NonTerminalSymbol> &start_symbol) { this->start_symbol = start_symbol; }
    void setProduction(const std::vector<std::shared_ptr<Symbol>> &production) { this->production = production; }
};

#endif // PRODUCTION_RULE_H