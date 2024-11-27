#include "CFG/ContextFreeGrammar.hpp"

// Constructor
ContextFreeGrammar::ContextFreeGrammar(const std::string &file_path)
{
    std::ifstream file(file_path);
    if (!file.is_open())
        throw std::runtime_error("No se pudo abrir el archivo.");

}

// Private functions
std::vector<std::string> ContextFreeGrammar::split(const std::string &line, char delimiter)
{
    std::stringstream ss(line);
    std::string item;
    std::vector<std::string> tokens;
    while (std::getline(ss, item, delimiter))
        tokens.push_back(item);
    return tokens;
}


// Display
void ContextFreeGrammar::display() const
{
    // Iterate non terminal
    for (const std::shared_ptr<NonTerminalSymbol> &non_terminal : non_terminal_symbols)
    {
        if (non_terminal == start_symbol)
            std::cout << "*";
        std::cout << non_terminal->getName() << "->";

        // Iterate prodcuctions
        for (const std::shared_ptr<ProductionRule> production : production_rules)
        {

            if (production->getStartSymbol() == non_terminal)
            {
                std::cout << "|";
                if (production->getProduction().empty())
                    std::cout << "E";
                else
                    for (const std::shared_ptr<Symbol> symbol : production->getProduction())
                        std::cout << symbol->getName();
            }
        }
        std::cout << std::endl;
    }
}