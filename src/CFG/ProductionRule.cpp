#include "CFG/ProductionRule.hpp"

bool ProductionRule::operator<(const ProductionRule &other) const
{
    // Comparar los símbolos iniciales
    if (*start_symbol < *other.start_symbol)
        return true;
    if (*other.start_symbol < *start_symbol)
        return false;

    // Comparar las producciones lexicográficamente
    size_t min_size = std::min(production.size(), other.production.size());
    for (size_t i = 0; i < min_size; ++i)
    {
        if (*production[i] < *other.production[i])
            return true;
        if (*other.production[i] < *production[i])
            return false;
    }

    // Si son iguales hasta el menor tamaño, el más corto es menor
    return production.size() < other.production.size();
}

void ProductionRule::display()
{
    std::cout << start_symbol->getName() << "->";

    for (const std::shared_ptr<Symbol> &symbol : production)
        std::cout << symbol->getName();

    std::cout << std::endl;
}