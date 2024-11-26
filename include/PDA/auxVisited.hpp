#ifndef AUX_VISITED_H
#define AUX_VISITED_H

#include <memory>
#include <optional>
#include "PDA/State.hpp"
#include "PDA/StackSymbol.hpp"

class auxVisited
{
private:
    std::shared_ptr<State> state;
    int chain_size;
    std::optional<std::shared_ptr<StackSymbol>> symbol;

public:
    // Constructores
    auxVisited(std::shared_ptr<State> state, int chain_size)
        : state(std::move(state)), chain_size(chain_size), symbol(std::nullopt) {}

    auxVisited(std::shared_ptr<State> state, int chain_size, std::shared_ptr<StackSymbol> symbol)
        : state(std::move(state)), chain_size(chain_size), symbol(std::make_optional(std::move(symbol))) {}

    // Sobrecarga de operator<
    bool operator<(const auxVisited &other) const
    {
        if (*state < *other.state)
            return true;
        if (*other.state < *state)
            return false;

        if (chain_size < other.chain_size)
            return true;
        if (other.chain_size < chain_size)
            return false;

        if (symbol.has_value() && other.symbol.has_value())
        {
            if (*symbol.value() < *other.symbol.value())
                return true;
            if (*other.symbol.value() < *symbol.value())
                return false;
        }
        else if (symbol.has_value() != other.symbol.has_value())
        {
            return symbol.has_value() < other.symbol.has_value();
        }

        return false;
    }

    // Getters

    // Setters
    void setSymbol(std::optional<std::shared_ptr<StackSymbol>> symbol) { this->symbol = symbol; }
};

#endif // AUX_VISITED_H
