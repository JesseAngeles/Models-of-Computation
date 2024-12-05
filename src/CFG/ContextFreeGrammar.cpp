#include "CFG/ContextFreeGrammar.hpp"

// Constructor
ContextFreeGrammar::ContextFreeGrammar(const std::string &file_path)
{
    std::ifstream file(file_path);
    if (!file.is_open())
        return;

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

void ContextFreeGrammar::eliminateUnitPairs()
{
    // Buscar aquellas producciones que tengan un simbolo no terminal diferente
    std::set<std::pair<std::shared_ptr<NonTerminalSymbol>, std::shared_ptr<NonTerminalSymbol>>> pairs;

    std::set<std::shared_ptr<ProductionRule>> pairs_delete;

    // Encontrar pares unitarios
    for (const std::shared_ptr<ProductionRule> &production_rule : production_rules)
        if (production_rule->getProduction().size() == 1)
            for (auto a : non_terminal_symbols)
                if (production_rule->getProduction()[0] == a)
                {
                    pairs.insert(std::make_pair(production_rule->getStartSymbol(), std::static_pointer_cast<NonTerminalSymbol>(production_rule->getProduction()[0])));
                    pairs_delete.insert(production_rule);
                    break;
                }

    for (const std::pair<std::shared_ptr<NonTerminalSymbol>, std::shared_ptr<NonTerminalSymbol>> &i : pairs)
        for (const std::pair<std::shared_ptr<NonTerminalSymbol>, std::shared_ptr<NonTerminalSymbol>> &j : pairs)
            if (i.second == j.first)
                pairs.insert(std::make_pair(i.first, j.second));

    for (const std::shared_ptr<ProductionRule> &production_rule : pairs_delete)
        production_rules.erase(production_rule);

    for (const std::pair<std::shared_ptr<NonTerminalSymbol>, std::shared_ptr<NonTerminalSymbol>> &pair : pairs)
    {
        if (pair.first == pair.second)
            continue;

        for (const std::shared_ptr<ProductionRule> &production_rule : production_rules)
            if (production_rule->getStartSymbol() == pair.second)
                production_rules.insert(std::make_shared<ProductionRule>(pair.first, production_rule->getProduction()));
    }
}

void ContextFreeGrammar::eliminateNonGeneratingSymbols()
{
    // Definir simbolos generadores
    std::set<std::shared_ptr<Symbol>> generating_symbols;

    for (const std::shared_ptr<TerminalSymbol> &terminal_symbol : terminal_symbols)
        generating_symbols.insert(terminal_symbol);

    int count;
    do
    {
        count = generating_symbols.size();

        // Iterar sobre las producciones
        for (const std::shared_ptr<ProductionRule> &production_rule : production_rules)
        {
            bool is_generating = true;

            // Iterar sobre los simbolos de la produccion
            for (const std::shared_ptr<Symbol> &symbol : production_rule->getProduction())
                if (generating_symbols.find(symbol) == generating_symbols.end())
                {
                    is_generating = false;
                    break;
                }

            if (is_generating)
                generating_symbols.insert(production_rule->getStartSymbol());
        }
    } while (count < generating_symbols.size());

    // Iterar nuevamente para borrar
    std::set<std::shared_ptr<ProductionRule>> productions_delete;
    std::set<std::shared_ptr<NonTerminalSymbol>> non_terminal_delete;

    for (const std::shared_ptr<ProductionRule> &production_rule : production_rules)
    {
        // Eliminar no generadores
        if (generating_symbols.find(production_rule->getStartSymbol()) == generating_symbols.end())
        {
            non_terminal_delete.insert(production_rule->getStartSymbol());
            continue;
        }

        // Eliminar producciones que no tengan generadores
        for (const std::shared_ptr<Symbol> &symbol : production_rule->getProduction())
            if (generating_symbols.find(symbol) == generating_symbols.end())
            {
                productions_delete.insert(production_rule);
                break;
            }
    }

    for (const std::shared_ptr<ProductionRule> &production_rule : productions_delete)
        production_rules.erase(production_rule);

    for (const std::shared_ptr<NonTerminalSymbol> &non_terminal : non_terminal_delete)
        non_terminal_symbols.erase(non_terminal);

    // for (const std::shared_ptr<NonTerminalSymbol> &non_terminal : non_terminal_symbols)
    //     if (generating_symbols.find(non_terminal) == generating_symbols.end())
    //         non_terminal_symbols.erase(non_terminal);
}

void ContextFreeGrammar::eliminateUnreachableSymbols()
{
    std::set<std::shared_ptr<Symbol>> achievable_symbols;
    achievable_symbols.insert(start_symbol);

    int count;
    do
    {
        count = achievable_symbols.size();

        // Iterar las producciones
        for (const std::shared_ptr<ProductionRule> &production_rule : production_rules)
            if (achievable_symbols.find(production_rule->getStartSymbol()) != achievable_symbols.end())
                // Iterar todos los simbolos y añadirlos a los achievable_symbols
                for (const std::shared_ptr<Symbol> &symbol : production_rule->getProduction())
                    achievable_symbols.insert(symbol);

    } while (count < achievable_symbols.size());

    // Eliminar las producciones que inicien con un alcanzable
    std::set<std::shared_ptr<ProductionRule>> production_delete;
    std::set<std::shared_ptr<NonTerminalSymbol>> non_terminal_delete;

    for (const std::shared_ptr<ProductionRule> &production_rule : production_rules)
        if (achievable_symbols.find(production_rule->getStartSymbol()) == achievable_symbols.end())
        {
            production_delete.insert(production_rule);
            non_terminal_delete.insert(production_rule->getStartSymbol());
        }

    std::set<std::shared_ptr<TerminalSymbol>> delete_determinals;
    for (const std::shared_ptr<ProductionRule> &production_rule : production_delete)
        production_rules.erase(production_rule);

    for (const std::shared_ptr<NonTerminalSymbol> &non_terminal : non_terminal_delete)
        non_terminal_symbols.erase(non_terminal);

    for (const std::shared_ptr<TerminalSymbol> &symbol : terminal_symbols)
        if (achievable_symbols.find(symbol) == achievable_symbols.end())
            terminal_symbols.erase(symbol);

    for (const std::shared_ptr<NonTerminalSymbol> &symbol : non_terminal_symbols)
        if (achievable_symbols.find(symbol) == achievable_symbols.end())
            non_terminal_symbols.erase(symbol);
}

void ContextFreeGrammar::clean()
{
    eliminateEpsilonProductions();
    eliminateUnitPairs();
    eliminateNonGeneratingSymbols();
    eliminateUnreachableSymbols();
}

// todo
void ContextFreeGrammar::chomskyForm()
{
    // Iterar todas las producciones
    for (const std::shared_ptr<ProductionRule> &production_rule : production_rules)
    {
        std::vector<std::shared_ptr<Symbol>> production = production_rule->getProduction();

        // Validar un unico simbolo no terminal
        if (production.size() == 1 &&
            terminal_symbols.find(std::static_pointer_cast<TerminalSymbol>(production[0])) != terminal_symbols.end())
            continue;

        // Dos simbolos no terminales
        if (production.size() == 2 &&
            non_terminal_symbols.find(std::static_pointer_cast<NonTerminalSymbol>(production[0])) != non_terminal_symbols.end() &&
            non_terminal_symbols.find(std::static_pointer_cast<NonTerminalSymbol>(production[1])) != non_terminal_symbols.end())
            continue;
            
        // Generar producciones de simbolos unitarias
        for (int i = 0; i < production.size(); i++)
            // Si es un simbolo terminal, intercambiar por otro
            if (terminal_symbols.find(std::static_pointer_cast<TerminalSymbol>(production[i])) != terminal_symbols.end())
            {
                // Nuevo nombre del estado
                std::string new_name = "V(" + production[i]->getName() + ")";
                std::shared_ptr<NonTerminalSymbol> new_symbol;

                // Buscar el nombre en los non_terminal_symbols
                bool finded = false;
                for (const std::shared_ptr<NonTerminalSymbol> &symbol : non_terminal_symbols)
                    if (symbol->getName() == new_name)
                    {
                        new_symbol = symbol;
                        finded = true;
                        break;
                    }

                if (!finded)
                {
                    new_symbol = std::make_shared<NonTerminalSymbol>(new_name);
                    non_terminal_symbols.insert(new_symbol);
                }

                // Crear la nueva produccion
                std::vector<std::shared_ptr<Symbol>> symbols;
                symbols.push_back(production[i]);
                std::shared_ptr<ProductionRule> new_production = std::make_shared<ProductionRule>(new_symbol, symbols);

                if (production_rules.find(new_production) == production_rules.end())
                    production_rules.insert(new_production);

                // Actualiza produccion
                production[i] = new_production->getStartSymbol();
            }

        while (production.size() > 2)
        {
            std::string new_name = "V(" + production[0]->getName() + "," + production[1]->getName() + ")";
            std::shared_ptr<NonTerminalSymbol> new_symbol;

            // Buscar el nombre en los non_terminal
            bool finded = false;
            for (const std::shared_ptr<NonTerminalSymbol> &symbol : non_terminal_symbols)
                if (symbol->getName() == new_name)
                {
                    new_symbol = symbol;
                    finded = true;
                    break;
                }

            if (!finded)
            {
                new_symbol = std::make_shared<NonTerminalSymbol>(new_name);
                non_terminal_symbols.insert(new_symbol);
            }

            // Crear nueva produccion
            std::vector<std::shared_ptr<Symbol>> symbols;
            symbols.push_back(production[0]);
            symbols.push_back(production[1]);
            std::shared_ptr<ProductionRule> new_production = std::make_shared<ProductionRule>(new_symbol, symbols);

            if (production_rules.find(new_production) == production_rules.end())
                production_rules.insert(new_production);

            // Actualizar produccion
            production[0] = new_production->getStartSymbol();
            production.erase(production.begin() + 1);
        }

        production_rule->setProduction(production);
    }

    char name = 'A';
    for (const std::shared_ptr<NonTerminalSymbol> &symbol : non_terminal_symbols)
    {
        if (symbol->getName() == "S")
            continue;
        if (name == 'E')
            name++;
        std::string new_name = "";
        new_name += name++;
        symbol->setName(new_name);
    }
}

PushdownAutomaton ContextFreeGrammar::toPDA()
{
    std::shared_ptr<State> q_0;
    std::set<std::shared_ptr<InputSymbol>> input_alphabet;
    std::set<std::shared_ptr<StackSymbol>> stack_alphabet;
    std::shared_ptr<StackSymbol> z_0;
    std::set<std::shared_ptr<Transition>> transitions;

    // Definir primer estado
    q_0 = std::make_shared<State>("q_0");

    // Definir el alfabeto de entrada
    for (const std::shared_ptr<TerminalSymbol> &symbol : terminal_symbols)
    {
        input_alphabet.insert(std::make_shared<InputSymbol>(symbol->getName()));
        stack_alphabet.insert(std::make_shared<StackSymbol>(symbol->getName()));
    }

    // Definir el alfabeto de pila
    for (const std::shared_ptr<NonTerminalSymbol> &symbol : non_terminal_symbols)
        stack_alphabet.insert(std::make_shared<StackSymbol>(symbol->getName()));

    // Definir fondo de pila
    for (const std::shared_ptr<StackSymbol> &symbol : stack_alphabet)
        if (symbol->getName() == start_symbol->getName())
        {
            z_0 = symbol;
            break;
        }

    // Definir produciones
    for (const std::shared_ptr<ProductionRule> &production_rule : production_rules)
    {
        // Generar todas las producciones
        std::shared_ptr<State> q = q_0;
        std::shared_ptr<StackSymbol> last_top;
        std::stack<std::shared_ptr<StackSymbol>> new_stack;

        for (const std::shared_ptr<StackSymbol> &symbol : stack_alphabet)
            if (symbol->getName() == production_rule->getStartSymbol()->getName())
                last_top = symbol;

        // Iterar simbolos de la produccion
        std::vector<std::shared_ptr<Symbol>> productions = production_rule->getProduction();
        for (int i = productions.size() - 1; i >= 0; --i)
        {
            // Encontrar el simbolo en las pilas
            for (const std::shared_ptr<StackSymbol> &symbol : stack_alphabet)
                if (symbol->getName() == productions[i]->getName())
                    new_stack.push(symbol);
        }

        std::shared_ptr<Transition> new_transition = std::make_shared<Transition>(q_0, q_0, std::nullopt, last_top, new_stack);
        transitions.insert(new_transition);
    };

    // Definir producciones de eliminacion
    for (const std::shared_ptr<TerminalSymbol> &symbol : terminal_symbols)
    {
        std::shared_ptr<InputSymbol> new_input;
        std::shared_ptr<StackSymbol> new_stack;
        // Buscar el simbolo en el alfabeto de entrada
        for (const std::shared_ptr<InputSymbol> &input : input_alphabet)
            if (symbol->getName() == input->getName())
                new_input = input;

        for (const std::shared_ptr<StackSymbol> &stack : stack_alphabet)
            if (symbol->getName() == stack->getName())
                new_stack = stack;

        std::shared_ptr<Transition> new_transition = std::make_shared<Transition>(q_0, q_0, new_input, new_stack, std::nullopt);
        transitions.insert(new_transition);
    }

    std::set<std::shared_ptr<State>> states;
    states.insert(q_0);

    std::set<std::shared_ptr<State>> final_states;

    return PushdownAutomaton(states, input_alphabet, stack_alphabet, transitions, q_0, z_0, final_states);
}

// Display
void ContextFreeGrammar::display() const
{
    std::cout << std::endl;
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