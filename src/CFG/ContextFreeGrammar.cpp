#include "CFG/ContextFreeGrammar.hpp"

// Constructor
ContextFreeGrammar::ContextFreeGrammar(const std::string &file_path)
{
    std::ifstream file(file_path);
    if (!file.is_open())
        throw std::runtime_error("No se pudo abrir el archivo.");

    std::string line;

    while (std::getline(file, line))
    {
        // Asignar el lado izquierdo
        std::vector<std::string> tokens = split(line, '-');

        bool is_start_symbol = false;
        std::string current_symbol = tokens[0];
        std::shared_ptr<NonTerminalSymbol> from_symbol;

        if (tokens[0][0] == '*')
        {
            current_symbol = tokens[0].substr(1);
            is_start_symbol = true;
        }

        bool finded = false;
        for (const std::shared_ptr<NonTerminalSymbol> &symbol : non_terminal_symbols)
            if (symbol->getName() == current_symbol)
            {
                finded = true;
                from_symbol = symbol;
                break;
            }

        if (!finded)
            from_symbol = std::make_shared<NonTerminalSymbol>(current_symbol);

        if (is_start_symbol)
            start_symbol = from_symbol;

        // Asignar el lado derecho
        std::vector<std::string> rules = split(tokens[1], '|');
        // Iterar cada posibilidad
        for (const std::string &rule : rules)
        {
            // Iterar cada production
            std::vector<std::shared_ptr<Symbol>> current_production;
            for (const char &sym : rule)
            {
                std::string symbol = "";
                symbol += sym;

                if (sym == 'E')
                {
                    current_production.clear();
                    break;
                }
                else if (std::isupper(sym))
                {
                    bool finded = false;
                    for (const std::shared_ptr<NonTerminalSymbol> &current : non_terminal_symbols)
                        if (current->getName() == symbol)
                        {
                            finded = true;
                            current_production.push_back(current);
                            break;
                        }

                    if (!finded)
                    {
                        std::shared_ptr<NonTerminalSymbol> new_symbol = std::make_shared<NonTerminalSymbol>(symbol);
                        non_terminal_symbols.insert(new_symbol);
                        current_production.push_back(new_symbol);
                    }
                }
                else
                {
                    bool finded = false;
                    for (const std::shared_ptr<TerminalSymbol> &current : terminal_symbols)
                        if (current->getName() == symbol)
                        {
                            finded = false;
                            current_production.push_back(current);
                            break;
                        }

                    if (!finded)
                    {
                        std::shared_ptr<TerminalSymbol> new_symbol = std::make_shared<TerminalSymbol>(symbol);
                        terminal_symbols.insert(new_symbol);
                        current_production.push_back(new_symbol);
                    }
                }
            }

            std::shared_ptr<ProductionRule> new_production = std::make_shared<ProductionRule>(from_symbol, current_production);
            production_rules.insert(new_production);

            // std::cout << "\nProduction: " << new_production->getStartSymbol()->getName() << "-> ";
            // for (auto c : new_production->getProduction())
            //     std::cout << c->getName() << " ";
        }
    }
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
    // // delete
    // std::cout << "\nFisrt symbol: " << start_symbol->getName();
    // std::cout << "\nTerminal symbols:\n";
    // for (auto c : terminal_symbols)
    //     std::cout << c->getName() << " ";
    // std::cout << "\nNon terminal symbols:\n";
    // for (auto c : non_terminal_symbols)
    //     std::cout << c->getName() << " ";

    // std::cout << "\nProductions: " << production_rules.size();

    // std::cout << "\nProductions: " << production_rules.size();
    for (auto c : non_terminal_symbols)
        if (c->getName() == start_symbol->getName())
            std::cout << "*";

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