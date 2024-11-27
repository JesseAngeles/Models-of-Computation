#include "CFG/ProductionRule.hpp"

bool ProductionRule::operator<(const ProductionRule &other) const
{
    if (start_symbol < other.start_symbol)
        return true;
    if (other.start_symbol < start_symbol)
        return false;


    if(production.size() == other.production.size())
        return true;

    for (int i = 0; i < production.size(); i++)
    {
        if (production[i] < other.production[i])
            return true;

        if (other.production[i] < production[i])
            return false;
    }

    return false;
}