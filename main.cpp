#include <iostream>
#include <memory>
#include <string>
#include "PDA/PushdownAutomaton.hpp"
#include "CFG/ContextFreeGrammar.hpp"

using namespace std;

#define DIVISOR "******************************************************"

void displayMenu()
{
    cout << DIVISOR << endl;
    cout << "              MENU PRINCIPAL" << endl;

    cout << "1)  Leer CFG" << endl;
    cout << "2)  Imprimir CFG" << endl;
    cout << "3)  Limpiar CFG" << endl;
    cout << "4)  Forma de Chomsky" << endl;
    cout << "5)  Convertir a PDA" << endl;
    cout << "6)  Leer PDA" << endl;
    cout << "7)  Imprimir PDA" << endl;
    cout << "8)  Convertir a pila vacia" << endl;
    cout << "9)  Convertir a estados finales" << endl;
    cout << "10) Convertir a CFG" << endl;
    cout << "11) Probar cadena" << endl;
    cout << "0)  Salir" << endl;
    cout << DIVISOR << endl;
    cout << "Selecciona una opción: ";
}

void displaySubMenu()
{
    cout << DIVISOR << endl;
    cout << "1) Producciones epsilon" << endl;
    cout << "2) Pares unitarios" << endl;
    cout << "3) No generadores" << endl;
    cout << "4) No alcanzables" << endl;
    cout << "0) Todos" << endl;
    cout << DIVISOR << endl;
    cout << "Selecciona una opción: ";
}

int main()
{
    int option = 0;

    std::shared_ptr<ContextFreeGrammar> cfg;
    std::shared_ptr<PushdownAutomaton> pda;

    do
    {
        displayMenu();
        cin >> option;

        switch (option)
        {
        case 1: // leer cfg
        {
            string file_name;
            cout << "Nombre del archivo: ";
            cin >> file_name;
            file_name = "./resources/CFG/" + file_name + ".csv";

            cfg = std::make_shared<ContextFreeGrammar>(file_name);
            cfg->display();
        }
        break;
        case 2:
            if (cfg)
                cfg->display();
            break;
        case 3:
        {
            int sub_option;
            displaySubMenu();
            cin >> sub_option;

            switch (sub_option)
            {
            case 1:
                cfg->eliminateEpsilonProductions();
                break;
            case 2:
                cfg->eliminateUnitPairs();
                break;
            case 3:
                cfg->eliminateNonGeneratingSymbols();
                break;
            case 4:
                cfg->eliminateUnreachableSymbols();
                break;
            default:
                cfg->clean();
                break;
            }

            cfg->display();
        }
        break;
        case 4:
        {
            cfg->chomskyForm();
            cfg->display();
        }
        break;
        case 5:
        {
            pda = std::make_shared<PushdownAutomaton>(cfg->toPDA());
            pda->display();
        }
        break;
        case 6:
        {
            string file_name;
            cout << "Nombre del archivo: ";
            cin >> file_name;
            file_name = "./resources/PDA/" + file_name + ".csv";

            pda = std::make_shared<PushdownAutomaton>(file_name);
            pda->display();
        }
        break;
        case 7:
        {
            pda->display();
        }
        break;
        case 8:
        {
            pda->final_state2empty_set();
            pda->display();
        }
        break;
        case 9:
        {
            pda->empty_set2final_state();
            pda->display();
        }
        break;
        case 10:
        {
            cfg = std::make_shared<ContextFreeGrammar>(pda->toCFG());
            cfg->display();
        }
        break;
        case 11:
        {
            
        }
        break;

        default:
            break;
        }

    } while (option);
}