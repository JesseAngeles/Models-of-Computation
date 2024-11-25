#include "PDA/Transition.hpp"

// Implementación del constructor principal
Transition::Transition(
    const State &start_state,
    const State &end_state,
    const std::optional<InputSymbol> &symbol,
    const StackSymbol &init_top,
    const std::stack<StackSymbol> &end_top)
    : start_state(start_state),
      end_state(end_state),
      init_top(init_top),
      end_top(end_top)
{
    if (symbol.has_value())
        this->symbol = symbol;
}

Transition::Transition(
    const State &start_state,
    const State &end_state,
    const std::optional<InputSymbol> &symbol,
    const StackSymbol &init_top,
    const std::optional<StackSymbol> &end_symbol)
    : start_state(start_state),
      end_state(end_state),
      init_top(init_top)
{
    if (symbol.has_value())
        this->symbol = symbol;

    if (end_symbol.has_value())
        this->end_top.push(end_symbol.value());
}

bool Transition::operator<(const Transition &other) const
{
    // Comparar el estado de inicio
    if (start_state < other.start_state)
        return true;
    if (other.start_state < start_state)
        return false;

    // Si los estados de inicio son iguales, comparar el estado final
    if (end_state < other.end_state)
        return true;
    if (other.end_state < end_state)
        return false;

    // Si los estados de inicio y final son iguales, comparar el símbolo de entrada
    if (symbol.has_value() && other.symbol.has_value())
    {
        if (symbol.value() < other.symbol.value())
            return true;
        if (other.symbol.value() < symbol.value())
            return false;
    }

    if (symbol.has_value() && !other.symbol.has_value())
        return true;
    if (other.symbol.has_value() && !symbol.has_value())
        return false;

    // Si el símbolo de entrada es igual, comparar el símbolo de la parte superior de la pila
    if (StackSymbol(init_top) < StackSymbol(other.init_top))
        return true;
    if (StackSymbol(other.init_top) < StackSymbol(init_top))
        return false;

    // Finalmente, comparar las pilas de transición (end_top)
    if (end_top.size() < other.end_top.size())
        return true;
    if (other.end_top.size() < end_top.size())
        return false;

    std::stack<StackSymbol> current_end_top = end_top;
    std::stack<StackSymbol> other_end_top = other.end_top;

    while (!current_end_top.empty())
    {
        if (current_end_top.top() < other_end_top.top())
            return true;
        if (other_end_top.top() < current_end_top.top())
            return false;

        current_end_top.pop();
        other_end_top.pop();
    }

    return false;
}