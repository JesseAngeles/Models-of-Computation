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
        {
            from_symbol = std::make_shared<NonTerminalSymbol>(current_symbol);
            non_terminal_symbols.insert(from_symbol);
        }

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
                            finded = true;
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

// Functions
void ContextFreeGrammar::eliminateEpsilonProductions()
{
    std::set<std::shared_ptr<ProductionRule>> delete_production;
    std::set<std::shared_ptr<NonTerminalSymbol>> epsilon_generators;
    std::set<std::shared_ptr<ProductionRule>> productions_afected;

    int last_size;
    do
    {
        last_size = epsilon_generators.size();

        // Iterar cada produccion
        for (const std::shared_ptr<ProductionRule> &production_rule : production_rules)
        {
            bool is_epsilon_generator = true;
            bool is_affected = false;

            if (production_rule->getProduction().empty())
            {
                epsilon_generators.insert(production_rule->getStartSymbol());
                delete_production.insert(production_rule);
            }

            // Iterar cada simbolo de la produccion
            for (const std::shared_ptr<Symbol> &symbol : production_rule->getProduction())
            {
                if (dynamic_cast<TerminalSymbol *>(symbol.get()))
                    is_epsilon_generator = false;

                if (std::shared_ptr<NonTerminalSymbol> non_terminal_symbol =
                        std::dynamic_pointer_cast<NonTerminalSymbol>(symbol))
                {
                    if (epsilon_generators.find(non_terminal_symbol) == epsilon_generators.end())
                        is_epsilon_generator = false;
                    else
                        is_affected = true;
                }
            }

            if (is_affected)
                productions_afected.insert(production_rule);

            if (is_epsilon_generator)
                epsilon_generators.insert(production_rule->getStartSymbol());
        }
    } while (last_size != epsilon_generators.size());

    // Iterar sobre cada produccion afectada
    for (const std::shared_ptr<ProductionRule> &production_rule : productions_afected)
    {
        int count = 0;
        // Iterar cada simbolo y buscar si es un generador de epsilon
        for (const std::shared_ptr<Symbol> &symbol : production_rule->getProduction())
            if (epsilon_generators.find(std::dynamic_pointer_cast<NonTerminalSymbol>(symbol)) != epsilon_generators.end())
                count++;

        int total = std::pow(2, count) - 1;
        std::vector<std::vector<std::shared_ptr<Symbol>>> new_productions(total);
        int binary_count = 1;

        // Iterar cada simbolo otra vez, ahora para crear todas las combinaciones
        for (const std::shared_ptr<Symbol> &symbol : production_rule->getProduction())
        {
            // Si no pertenece a los generadores de epsilon, agregar a todos
            if (epsilon_generators.find(std::static_pointer_cast<NonTerminalSymbol>(symbol)) == epsilon_generators.end())
            {
                for (std::vector<std::shared_ptr<Symbol>> &production : new_productions)
                    production.push_back(symbol);
            }
            // Si pertenece a los generadores de epsilon, agregar a la mitad
            else
            {
                bool add = true;
                for (int i = 0; i < total; i++)
                {
                    if (i % binary_count == 0)
                        add = !add;

                    if (add)
                        new_productions[i].push_back(symbol);
                }
                binary_count *= 2;
            }
        }

        // Asginar nuevas transiciones
        for (const std::vector<std::shared_ptr<Symbol>> &new_production : new_productions)
        {
            if (new_production.empty())
                continue;

            production_rules.insert(std::make_shared<ProductionRule>(production_rule->getStartSymbol(), new_production));
        }
    }

    // Eliminar transiciones epsilon
    for (const std::shared_ptr<ProductionRule> &production_rule : delete_production)
        production_rules.erase(production_rule);
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