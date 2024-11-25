#include "PDA/Transition.hpp"

// Implementación del constructor principal
Transition::Transition(
    const std::shared_ptr<State> &start_state,
    const std::shared_ptr<State> &end_state,
    const std::optional<std::shared_ptr<InputSymbol>> &symbol,
    const std::shared_ptr<StackSymbol> &init_top,
    const std::stack<std::shared_ptr<StackSymbol>> &end_top)
    : start_state(start_state),
      end_state(end_state),
      init_top(init_top),
      end_top(end_top)
{
    if (symbol.has_value())
        this->symbol = symbol;
}

Transition::Transition(
    const std::shared_ptr<State> &start_state,
    const std::shared_ptr<State> &end_state,
    const std::optional<std::shared_ptr<InputSymbol>> &symbol,
    const std::shared_ptr<StackSymbol> &init_top,
    const std::optional<std::shared_ptr<StackSymbol>> &end_symbol)
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
    if (init_top < other.init_top)
        return true;
    if (other.init_top < init_top)
        return false;

    // Finalmente, comparar las pilas de transición (end_top)
    if (end_top.size() < other.end_top.size())
        return true;
    if (other.end_top.size() < end_top.size())
        return false;

    std::stack<std::shared_ptr<StackSymbol>> current_end_top = end_top;
    std::stack<std::shared_ptr<StackSymbol>> other_end_top = other.end_top;

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

// Display
void Transition::display() const
{
    std::cout << "\n(" << start_state->getName() << ", " << end_state->getName() << ") ->";

    symbol.has_value() ? std::cout << symbol.value()->getName() : std::cout << "E";

    std::cout << "," << init_top->getName() << "/";

    std::stack<std::shared_ptr<StackSymbol>> copy_stack = end_top; 
    if (!copy_stack.empty())
        while(!copy_stack.empty())
            {
                std::cout << copy_stack.top()->getName() << " ";
                copy_stack.pop();
            }
    else
        std::cout << "E";
}