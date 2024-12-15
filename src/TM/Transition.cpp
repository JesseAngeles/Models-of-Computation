#include "TM/Transition.hpp"

// Constructor
Transition::Transition(
    const std::shared_ptr<State> &start_state,
    const std::shared_ptr<State> &end_state,
    const std::shared_ptr<Symbol> &start_symbol,
    const std::shared_ptr<Symbol> &end_symbol,
    const Direction &direction)
    : start_state(start_state),
      end_state(end_state),
      start_symbol(start_symbol),
      end_symbol(end_symbol),
      direction(direction) {}

// Overloading
bool Transition::operator<(const Transition &other) const
{
    // Compare start_state
    if (start_state < other.start_state)
        return true;
    if (other.start_state < start_state)
        return false;

    // Compare end_state
    if (end_state < other.end_state)
        return true;
    if (other.end_state < end_state)
        return false;

    // Compare start_symbol
    if (start_symbol < other.start_symbol)
        return true;
    if (other.start_symbol < start_symbol)
        return false;

    // Compare end_symbol
    if (end_symbol < other.end_symbol)
        return true;
    if (other.end_symbol < end_symbol)
        return false;

    // Compare directions
    if (direction != other.direction)
        return true;

    return false;
}

// Display
void Transition::display() const
{
    std::cout << "(" << start_state->getName() << " * " << start_symbol->getName() << ") = ";
    std::cout << end_state->getName() << " * " << end_symbol->getName() << " * ";
    std::cout << (direction == Direction::Left ? "L" : "R") << std::endl;
}