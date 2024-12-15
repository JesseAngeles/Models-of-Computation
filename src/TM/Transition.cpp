#include "TM/Transition.hpp"

// Constructor
Transition::Transition(
    const std::shared_ptr<State> &start_state,
    const std::shared_ptr<State> &end_state,
    const std::optional<std::shared_ptr<Symbol>> &start_symbol,
    const std::optional<std::shared_ptr<Symbol>> &end_symbol,
    const Direction &direction)
    : start_state(start_state),
      end_state(end_state),
      start_symbol(start_symbol),
      end_symbol(end_symbol) {}

void Transition::display() const
{
  std::cout << "(" << start_state->getName() << " * ";
  std::cout << (start_symbol.has_value() ? start_symbol.value()->getName() : "B") << ") -> ";
  std::cout << end_state->getName() << " * ";
  std::cout << (end_symbol.has_value() ? end_symbol.value()->getName() : "B") << " * ";
  std::cout << (direction == Direction::Left ? "L" : "R") << std::endl;
}