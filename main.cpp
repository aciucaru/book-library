#include <iostream>
#include <vector>
#include <deque>
#include <string>

#include "ListaLegata.h"
#include "Biblioteca.h"

/* functie utilitara ce elimina spatiile albe numai de la inceputul si de la sfarsitul unui string 
   exemplu de functionare:
   - eliminaSpatiiAlbe("     C++ Programming Language   "): "C++ Programming Language"
   - eliminaSpatiiAlbe("   Bjarne Stroustrup"): "Bjarne Stroustrup"
   - eliminaSpatiiAlbe("C++: Effective Modern C++      "): "C++: Effective Modern C++"
   - eliminaSpatiiAlbe("Paul Laurence"): "Paul Laurence" (fara efect, in acest caz) */
std::string eliminaSpatiiAlbe(std::string& token)
{
    /* aceasta functie functioneaza in felul urmator:
       - se determina indexul la care se termina spatiile albe de la inceputul string-ului
       - se determina indexul la care incep spatiile albe de la sfarsitul stringului
       - intre aceste doua index-uri va fi stringul fara spatii albe de inceput si sfarsit, adica
        string-ul de returnat */
    std::string rezultat;
    
    if(token.length() == 0)
        rezultat = "";
    else
    {
        // se gaseste indexul primei litere ce nu este spatiu " "
        auto index_inceput_rezultat = token.find_first_not_of(" ");
        // se gaseste indexul ultimei litere ce nu este spatiu " "
        auto index_sfarsit_rezultat = token.find_last_not_of(" ");
        
        // daca nu s-a gasit nimic diferit de " ", inseamna ca token-ul are doar spatii albe sau este gol
        if(index_inceput_rezultat == std::string::npos)
            rezultat = ""; // in acest caz se returneaza un string gol
        else
        {
            auto lungime_rezultat = index_sfarsit_rezultat - index_inceput_rezultat + 1;
            // se extrage string-ul necesar
            rezultat = token.substr(index_inceput_rezultat, lungime_rezultat);
        }
    }
    return rezultat;
}

// functie utilitara ce desparte o intreaga linie in token-urile (cuvintele) ei (adica in litera comenzii si argumente)
std::vector<std::string> desparteLinieComanda(std::string& linie_comanda)
{
    /* pentru acest program, o comanda arata astfel: C arg1, arg2, arg3, arg4, ...
     * unde:
     * - C este litera comenzii (A, L, E, I, P)
     * - arg1, arg2, arg3, arg4, ... sunt argumentele comenzii (unele comenzi pot sa nu aiba argumente)
     * aceasta functie desparte intreaga linie de comanda in aceste elemente si le returneaza ca vector
     * */
    
    /* pentru ca trebuie inserate elemente si la inceputul sirului, se va lucra cu un deque, iar apoi
       se va converti deque-ul intr-un vector (vectorul va fi de fapt returnat de aceasta functie) */
    std::deque<std::string> deque_token; // sir cu care se va lucra efectiv
    std::vector<std::string> vector_token; // sir ce va fi returnat
    
    // pasul 1. se separa string-ul liniei de comanda dupa delimitatorul ','
    std::stringstream s_stream(linie_comanda);
    std::string token_curent;
    char delimitator = ',';
    while(std::getline(s_stream, token_curent, delimitator))
    {
        deque_token.push_back(token_curent);
//      vector_token.push_back(std::move(item)); // C++11
    }
    /* dupa separare, de exemplu, comanda 'A 10, C++ Programming Language, Bjarne Stroustrup, 2013' va fi:
            A 10
            Bjarne Stroustrup
            C++ Programming Language
            2013 
       se observa ca litera comenzii (A) este amestecata cu primul argument (adica 10); aceasta problema se
       rezolva in pasul urmator */
    
    // pasul 2. se separa litera comenzii de primul argument
    if(deque_token.size() > 0) // daca sirul nu este gol
    {
        // atunci se ia primul element (care sigur exista)
        std::string primul_token = deque_token.front();
        // se elimina primul element, deoarece el reprezinta de fapt 2 elemente si va fi inlocuit cu cele 2 elemente separate
        deque_token.pop_front();
        
        /* acum ca s-a obtinut primul token inainte de virgula, exista doua posibilitati:
           - comanda este de tip 'fara argumente', deci primul token contine doar un cuvant (comanda in sine)
           - comanda este de tip 'cu argumente', deci primul token contine 2 cuvinte (comanda si un argument) */
        
        /* ca sa se afle daca primul token contine unul sau doua cuvinte, se testeaza existenta spatiilor deoarece,
           daca sunt mai multe cuvinta, garantat exista macar un spatiu */
        if(primul_token.find(' ') != std::string::npos) // daca exista macar un spatiu
        {
            // atunci primul token contine o comanda si un argument
            std::string comanda = primul_token.substr(0, primul_token.find(' '));
            std::string primul_argument = primul_token.substr(primul_token.find(' '), primul_token.length() - 1);
            
//             // se extrage fiecare parte a primului token
//             std::stringstream stream_primul_token(primul_token);
//             stream_primul_token >> comanda; // se extrage primul token(cuvant) in string-ul 'comanda'
//             stream_primul_token >> primul_argument; // se extrage al doilea token in string-ul 'primul_argument'
            
            // se adauga string-urile corecte la inceputul deque-ului de token-uri
            deque_token.push_front(primul_argument);
            deque_token.push_front(comanda);
        }
        else
        {
            // atunci primul token contine doar comanda
            std::string comanda = primul_token.substr(0, primul_token.find(' '));
            
//             std::stringstream stream_primul_token(primul_token);
//             stream_primul_token >> comanda;
            
            // se adauga string-ul comenzii la inceputul deque-ului de token-uri
            deque_token.push_front(comanda); 
        }
        /* acum comanda 'A 10, C++ Programming Language, Bjarne Stroustrup, 2013' ar trebui sa fie despartita asa:
            A
            10
            Bjarne Stroustrup
            C++ Programming Language
            2013 */
    }
    
    // pasul 3. se elimina spatiile albe de la inceputul si sfarsitul fiecarui string, ramase in urma separarii
    // liniei de comanda
    // si, pentru ca este mai comod sa se lucreze cu vectori, deque-ul obtinut se converteste intr-un vector
    // si, in final, se va returna vectorul
    for(std::string token_curent : deque_token) // pentru fiecare string din deque
    {
         vector_token.push_back(eliminaSpatiiAlbe(token_curent));
    }

    return vector_token;
}

// functie ce determina daca un cuvant (de tip string) este cu adevarat o comanda valida
// iar daca este o comanda valida, functia extrage si returneaza comanda sub forma de char, ca sa nu mai
// fie nevoie de inca o functie care sa extraga litara comenzii din cuvant
char extrageComanda(const std::string& token)
{
    
    char litera_comanda = '\0';
    
    // daca token-ul (cuvantul) are exact un caracter, inseamna ca ar putea fi o comanda (comenzile au un singur caracter)
    if(token.length() == 1)
    {
        const int NUMAR_COMENZI = 7;
        const char comenzi_posibile[NUMAR_COMENZI] = {'A', 'L', 'E', 'I', 'C', 'P', 'S'};
        
        // se verifica daca token-ul (cuvantul) primit (ce are sigur un caracter) contine una dintre comenzile comenzi_posibile
        for(int i=0; i<NUMAR_COMENZI; i++)
        {
            if(token.front() == comenzi_posibile[i])
            {
                litera_comanda = token.front();
                break; // daca s-a gasit o asociere, se intrerupe bucla for
            }
        }
    }
    
    return litera_comanda;
}

// functii pentru validarea argumentelor primite de la utilizator
// fiecare comanda are o functie de validare asociata, ce valideaza argumentele specifice acelei comenzi
bool valideazaArgAdaugareCarte(std::vector<std::string>& sir_argumente)
{
    // variabila ce stocheaza rezultatul validarii
    // la inceput, se presupune ca argumentele sunt corecte; la orice abatere, rezultatul va deveni fals
    bool rezultat_validare = true;
    
    // variabile ce reprezinta argumentele pasate, sunt variabilele ce vor fi validate intr-un final
    int id;
    std::string string_autor;
    std::string string_titlu;
    int an_publicatie;
    
    // 1. pentru comanda de adaugare carte, este nevoie de 4 argumente
    // se verifica daca sirul de argumente are intr-adevar 4 argumente
    if(sir_argumente.size() == 4) // daca numarul de argumente este cel corect, atunci se pot verifica si argumentele in sine
    {
        // 2. se verifica daca argumentul 'id carte' este numar, nu un text
        // verificarea se face pe baza de exceptii, cu metoda 'stoi()' ce arunca exceptie daca string-ul nu este un numar
        std::string string_id = sir_argumente.at(0); // string-ul coresp. argumentului 'id carte' (este primul argument)
        try
        {
            id = std::stoi(string_id);
            
            // pe ramura 'try' nu exista nici o exceptie, deci cu siguranta 'stoi()' a returnat un nr. intreg
            // dar id-ul nu poate fi orice numar intreg, trebuie sa fie pozitiv
            if(id < 0)
                rezultat_validare = false; // nu trece de validare daca este negativ
        }
        // pe ramura 'catch', se stie garantat ca string-ul nu reprezenta un int, deci nu trece de validare
        catch(std::exception& excep) { rezultat_validare = false; }
        
        // 3. se verifica daca argumentul 'nume autor' este gol
        string_autor = sir_argumente.at(1); // numele autorului este al 3-lea argument
        if(string_autor.empty())
            rezultat_validare = false;
        
        // 4. se verifica daca argumentul 'titlu carte' este gol
        string_titlu = sir_argumente.at(2); // titlul este al 2-lea argument
        if(string_titlu.empty())
            rezultat_validare = false;
        
        // 5. se verifica daca argumentul 'an publicatie' este numar, nu un text
        // verificarea se face pe baza de exceptii, cu metoda 'stoi()' ce arunca exceptie daca string-ul nu este un numar
        std::string string_an = sir_argumente.at(3); // string-ul coresp. argumentului 'an_publicatie' (al 4-lea argument)
        try
        {
            an_publicatie = std::stoi(string_an);
            
            // pe ramura 'try' nu exista nici o exceptie, deci cu siguranta 'stoi()' a returnat un nr. intreg
            // dar id-ul nu poate fi orice numar intreg, trebuie sa fie pozitiv
            if(an_publicatie < 0)
                rezultat_validare = false; // nu trece de validare daca este negativ
        }
        // pe ramura 'catch', se stie garantat ca string-ul nu reprezenta un int, deci nu trece de validare
        catch(std::exception& excep) { rezultat_validare = false; }
    }
    else
        rezultat_validare = false;
    
    return rezultat_validare;
}

bool valideazaArgIdCarte(std::vector<std::string>& sir_argumente)
{
    // variabila ce stocheaza rezultatul validarii
    // la inceput, se presupune ca argumentele sunt corecte; la orice abatere, rezultatul va deveni fals
    bool rezultat_validare = true;
    
    // variabile ce reprezinta singurul argument pasat, este folosita pentru validarea finala
    int id;
    
    // 1. pentru comenzile ce au ca argument doar id-ul cartii, este nevoie de 1 argument
    // se verifica daca sirul de argumente are intr-adevar 1 argument
    if(sir_argumente.size() == 1) // daca numarul de argumente este cel corect, atunci se pot verifica si argumentele in sine
    {
        // 2. se verifica daca argumentul 'id carte' este numar, nu un text
        // verificarea se face pe baza de exceptii, cu metoda 'stoi()' ce arunca exceptie daca string-ul nu este un numar
        std::string string_id = sir_argumente.at(0); // string-ul coresp. argumentului 'id carte' (este primul argument)
        try
        {
            id = std::stoi(string_id);
            
            // pe ramura 'try' nu exista nici o exceptie, deci cu siguranta 'stoi()' a returnat un nr. intreg
            // dar id-ul nu poate fi orice numar intreg, trebuie sa fie pozitiv
            if(id < 0)
                rezultat_validare = false; // nu trece de validare daca este negativ
        }
        // pe ramura 'catch', se stie garantat ca string-ul nu reprezenta un int, deci nu trece de validare
        catch(std::exception& excep) { rezultat_validare = false; }
    }
    else
        rezultat_validare = false;
    
    return rezultat_validare;
}

bool valideazaArgImprumutareCarte(std::vector<std::string>& sir_argumente)
{
    // variabila ce stocheaza rezultatul validarii
    // la inceput, se presupune ca argumentele sunt corecte; la orice abatere, rezultatul va deveni fals
    bool rezultat_validare = true;
    
    // variabile ce reprezinta argumentele pasate, sunt variabilele ce vor fi validate intr-un final
    int id;
    std::string nume_persoana;
    
    // 1. pentru comanda de imprumutare carte, este nevoie de 2 argumente
    // se verifica daca sirul de argumente are intr-adevar 2 argumente
    if(sir_argumente.size() == 2)  // daca numarul de argumente este cel corect, atunci se pot verifica si argumentele in sine
    {
        // 2. se verifica daca argumentul 'id carte' este numar, nu un text
        // verificarea se face pe baza de exceptii, cu metoda 'stoi()' ce arunca exceptie daca string-ul nu este un numar
        std::string string_id = sir_argumente.at(0); // string-ul coresp. argumentului 'id carte' (este primul argument)
        try
        {
            id = std::stoi(string_id);
            
            // pe ramura 'try' nu exista nici o exceptie, deci cu siguranta 'stoi()' a returnat un nr. intreg
            // dar id-ul nu poate fi orice numar intreg, trebuie sa fie pozitiv
            if(id < 0)
            {
                std::cout << "valideazaArgImprumutareCarte(): id negativ" << std::endl;
                rezultat_validare = false; // nu trece de validare daca este negativ
            }
        }
        // pe ramura 'catch', se stie garantat ca string-ul nu reprezenta un int, deci nu trece de validare
        catch(std::exception& excep)
        {
            std::cout << "valideazaArgImprumutareCarte(): string-ul nu este un int" << std::endl;
            rezultat_validare = false;
        }
        
        // 3. se verifica daca argumentul 'nume persoana' este gol
        nume_persoana = sir_argumente.at(1); // numele persoanei este al 2-lea argument
        if(nume_persoana.empty())
        {
            std::cout << "valideazaArgImprumutareCarte(): nume persoana gol" << std::endl;
            rezultat_validare = false;
        }
    }
    else
    {
        std::cout << "valideazaArgImprumutareCarte(): numar argumente incorect: " << sir_argumente.size() << std::endl;
        rezultat_validare = false;
    }
    
    return rezultat_validare;
}

bool valideazaArgNumePersoana(std::vector<std::string>& sir_argumente)
{
    // variabila ce stocheaza rezultatul validarii
    // la inceput, se presupune ca argumentele sunt corecte; la orice abatere, rezultatul va deveni fals
    bool rezultat_validare = true;
    
    // variabile ce reprezinta argumentele pasate, sunt variabilele ce vor fi validate intr-un final
    std::string nume_persoana;
    
    // 1. pentru comenzile ce au ca argument numele unei persoane, este nevoie de 1 argument
    // se verifica daca sirul de argumente are intr-adevar 1 argument
    if(sir_argumente.size() == 1) // daca numarul de argumente este cel corect, atunci se pot verifica si argumentele in sine
    {
        // 2. se verifica daca argumentul 'nume autor' este gol
        nume_persoana = sir_argumente.at(0); // numele autorului este al 3-lea argument
        if(nume_persoana.empty())
            rezultat_validare = false;
    }
    else
        rezultat_validare = false;
    
    return rezultat_validare;
}

int main()
{
    Biblioteca* biblioteca;
    int numar_maxim_carti;
    
    // cat timp aceasta variabila este adevarata, programul continua sa accepte comenzi
    bool continua_rularea = true;
    
    std::cout << "Acest program simuleaza o bibliotecade carti" << std::endl;
    
    std::cout << "Va rugam introduceti numarul maxim permis de carti (pentru nelimitat introduceti 0): ";
    std::cin >> numar_maxim_carti;
    // dupa ce se ia un numar, trebuie folosit ignore(), altfel urmatorul string nu se poate lua corect
    std::cin.ignore();
    biblioteca = new Biblioteca(numar_maxim_carti);
    
    std::cout << std::endl;
    std::cout << "Lista de comenzi disponibile:" << std::endl;
    std::cout << "A id, autor, nume, an - adaugarea unei carti in biblioteca" << std::endl;
    std::cout << "L - listarea tuturor cartilor din biblioteca -" << std::endl;
    std::cout << "E id - eliminarea unei carti din bibilioteca" << std::endl;
    std::cout << "I id, nume_persoana - imprumutul unei carti catre o persoana" << std::endl;
    std::cout << "P nume_persoana - listarea cartilor imprumutate de o persoana" << std::endl;
    std::cout << "S - stop" << std::endl;
    
    std::cout << std::endl;
    std::cout << "exemple de utilizare:" << std::endl;
    std::cout << "A 10, Bjarne Stroustrup, C++ Programming Language, 2013" << std::endl;
    std::cout << "   adauga o carte cu id 10, autor 'Bjarne Stroustrup', titlu 'C++ Programming Language', an 2013" << std::endl;
    
    std::cout << "E 8" << std::endl;
    std::cout << "   elimina cartea cu id 8 (daca aceasta exista)" << std::endl;
    
    std::cout << "I 10, Dan" << std::endl;
    std::cout << "   imprumuta cartea cu id 10 persoanei cu numele 'Dan'" << std::endl;
    
    std::cout << "P Dan" << std::endl;
    std::cout << "   afiseaza cartile imprumutate de persoana 'Dan'" << std::endl;
    std::cout << "restul comenzilor nu au argumente" << std::endl;
    
    std::string linie_comanda_curenta = ""; // string ce contine intreaga linie introdusa de utilizator
    std::vector<std::string> sir_token; // sir ce contine linia impartita in componentele ei (comanda + argumente)
    char litera_comanda_curenta; // litera corespunzatoare unei comenzi, se axtrage din primul string al sirului 'sir_token'
    std::vector<std::string> sir_argumente; // sir ce contine doar argumentele unei comenzi, se extrage din 'sir_token'
    
    std::cout << std::endl;
    while(continua_rularea)
    {
        std::cout << "introduceti o comanda: ";
        getline(std::cin, linie_comanda_curenta);
        
        sir_token = desparteLinieComanda(linie_comanda_curenta);
        
        // daca s-a introdus o comanda cu macar un cuvant
        if(sir_token.size() > 0)
        {
            // primul cuvant al comenzii este primul element al sirului 'sir_token' si se poate lua cu 'front()'
            // daca acel cuvant nu este o comanda, metoda 'extrageComanda()' va returna caracterul null
            litera_comanda_curenta = extrageComanda(sir_token.front());
            
            switch(litera_comanda_curenta)
            {
                case 'A': // adaugarea unei carti
                {
                    // daca linia comenzii are 5 cuvinte (token-uri) (adica o comanda si 4 argument)
                    if(sir_token.size() == 5)
                    {
                        // se extrage sirul de argumente din sirul de cuvinte (token-uri)
                        // extragerea se face de la al 2-lea pana la ultimul element
                        // deoarece primul element este chiar comanda in sine, nu este un argument al comenzii
                        sir_argumente = std::vector<std::string>(sir_token.begin() + 1, sir_token.end());
                        
                        // se verifica daca argumentele sunt valide
                        bool argumente_valide = valideazaArgAdaugareCarte(sir_argumente);
                        
                        if(argumente_valide) // daca argumentele sunt valide
                        {
                            // atunci se iau valorile corepsunzatoare argumentelor
                            int id = std::stoi(sir_argumente.at(0));
                            std::string autor = sir_argumente.at(1);
                            std::string titlu = sir_argumente.at(2);
                            int an_publicatie = std::stoi(sir_argumente.at(3));
                            
                            // si se construieste un obiect de tip 'Carte', care apoi se adauga la biblioteca
                            Carte* carte_noua = new Carte(id, autor, titlu, an_publicatie);
                            biblioteca->adauga(carte_noua);
                            std::cout << "carte adaugata cu succes: " << carte_noua->catreString() << std::endl;
                        }
                        else
                        {
                            std::cout << "eroare: argumentele pentru comanda 'A' nu sunt valide, va rugam incercati din nou" << std::endl;
                            std::cout << "sugestie: argumentele trebuie separate prin virgula" << std::endl;
                        }
                    }
                    else
                        std::cout << "eroare: argumentele pentru comanda 'A' nu sunt valide, va rugam incercati din nou" << std::endl;
                }
                    break;
                
                case 'L': // listarea tuturor cartilor
                    // daca linia comenzii are exact un cuvant (adica nu are argumente)
                    if(sir_token.size() == 1)
                    {

                        std::cout << "cartile sunt:" << std::endl;
                        biblioteca->listeazaCarti();
                    }
                    else
                        std::cout<< "eroare: comanda 'L' nu trebuie sa aiba argumente, va rugam incercati din nou" << std::endl;
                    break;
                
                case 'E': // eliminarea unei carti
                {
                    // daca linia comenzii are 2 cuvinte (token-uri) (adica o comanda si un argument)
                    if(sir_token.size() == 2)
                    {
                        // se extrage sirul de argumente din sirul de cuvinte (token-uri)
                        // extragerea se face de la al 2-lea pana la ultimul element
                        sir_argumente = std::vector<std::string>(sir_token.begin() + 1, sir_token.end());
                        
                        // se verifica daca argumentele pasate pentru id-ul cartii sunt corecte
                        // in realitate ar trebui sa fie un singur argument, iar metoda valideazaArgIdCarte() ia in
                        // considerare acest lucru si da eroare daca detecteaza mai multe argumente
                        bool argumente_valide = valideazaArgIdCarte(sir_argumente);
                        
                        if(argumente_valide) // daca argumentul este valid (este doar unul singur)
                        {
                            // atunci se iau valorile corepsunzatoare argumentelor
                            int id = std::stoi(sir_argumente.at(0));
                            
                            // si se incearca eliminarea cartii din biblioteca
                            // metoda 'eliminaCarte()' returneaza adevarat daca a eliminat cartea si fals daca nu
                            bool rezultat_eliminare = biblioteca->eliminaCarte(id);
                            
                            if(rezultat_eliminare == true)
                                std::cout << "carte eliminata cu succes" << std::endl;
                            else
                                std::cout << "cartea cu id " << id << " nu s-a sters (nu se gaseste in lista sau este deja imprumutata)" << std::endl;
                        }
                        else
                            std::cout << "eroare: argumentele pentru comanda 'E' nu sunt valide, va rugam incercati din nou" << std::endl;
                    }
                    else
                    {
                        std::cout << "eroare: argumentele pentru comanda 'E' nu sunt valide, va rugam incercati din nou" << std::endl;
                        std::cout << "sugestie: argumentele trebuie separate prin virgula" << std::endl;
                    }
                }
                    break;
                    
                case 'I': // imprumutul unei carti
                {
                    // daca linia comenzii are 3 cuvinte (token-uri) (adica o comanda si 2 argumente)
                    if(sir_token.size() == 3)
                    {
                        // se extrage sirul de argumente din sirul de cuvinte (token-uri)
                        // extragerea se face de la al 2-lea pana la ultimul element
                        sir_argumente = std::vector<std::string>(sir_token.begin() + 1, sir_token.end());
                        
                        // se verifica daca argumentele pasate pt. imprumutarea cartii sunt corecte
                        bool argumente_valide = valideazaArgImprumutareCarte(sir_argumente);
                        
                        if(argumente_valide) // daca argumentul (este doar unul singur) este valid 
                        {
                            // atunci se iau valorile corespunzatoare argumentelor
                            int id = std::stoi(sir_argumente.at(0));
                            std::string nume_persoana = sir_argumente.at(1);
                            
                            // si se incearca imprumutarea cartii
                            // metoda 'imprumutaCarte()' returneaza adevarat doar daca a imprumutat cartea
                            bool rezultat_imprumutare = biblioteca->imprumutaCarte(id, nume_persoana);
                            
                            if(rezultat_imprumutare == true)
                                std::cout << "carte imprumutata cu succes" << std::endl;
                            else
                                std::cout << "cartea cu id " << id << " nu s-a imprumutat (fie nu exista, fie este deja imprumutata)" << std::endl;
                        }
                        else
                            std::cout << "eroare: argumentele pentru comanda 'I' nu sunt valide, va rugam incercati din nou" << std::endl;
                    }
                    else
                    {
                        std::cout << "eroare: argumentele pentru comanda 'I' nu sunt valide, va rugam incercati din nou" << std::endl;
                        std::cout << "sugestie: argumentele trebuie separate prin virgula" << std::endl;
                    }
                }
                    break;
                    
                case 'C': // listarea informatiilor despre o carte
                {
                    // daca linia comenzii are 2 cuvinte (token-uri) (adica o comanda si un argument)
                    if(sir_token.size() == 2)
                    {
                        // se extrage sirul de argumente din sirul de cuvinte (token-uri)
                        // extragerea se face de la al 2-lea pana la ultimul element
                        sir_argumente = std::vector<std::string>(sir_token.begin() + 1, sir_token.end());
                        
                        // se verifica daca argumentele pasate pentru id-ul cartii sunt corecte
                        // in realitate ar trebui sa fie un singur argument, iar metoda valideazaArgIdCarte() ia in
                        // considerare acest lucru si da eroare daca detecteaza mai multe argumente
                        bool argumente_valide = valideazaArgIdCarte(sir_argumente);
                        
                        if(argumente_valide) // daca argumentul (este doar unul singur) este valid 
                        {
                            // atunci se iau valorile corepsunzatoare argumentelor
                            int id = std::stoi(sir_argumente.at(0));
                            
                            // si se incearca listarea informatiilor cartii
                            // metoda 'listeazaInfoCarte(id)' listeaza informatiile doar daca id-ul exista cu adevarat
                            biblioteca->listeazaInfoCarte(id);
                        }
                        else
                            std::cout << "eroare: argumentele pentru comanda 'C' nu sunt valide, va rugam incercati din nou" << std::endl;
                    }
                    else
                    {
                        std::cout << "eroare: argumentele pentru comanda 'C' nu sunt valide, va rugam incercati din nou" << std::endl;
                        std::cout << "sugestie: argumentele trebuie separate prin virgula" << std::endl;
                    }
                }
                    break;
                
                case 'P': // listarea informatiilor despre o persoana
                {
                    // daca linia comenzii are 2 cuvinte (token-uri) (adica o comanda si un argument)
                    if(sir_token.size() == 2)
                    {
                        // se extrage sirul de argumente din sirul de cuvinte (token-uri)
                        // extragerea se face de la al 2-lea pana la ultimul element
                        sir_argumente = std::vector<std::string>(sir_token.begin() + 1, sir_token.end());
                        
                        // se verifica daca argumentele pasate pentru id-ul cartii sunt corecte
                        // in realitate ar trebui sa fie un singur argument, iar metoda valideazaArgIdCarte() ia in
                        // considerare acest lucru si da eroare daca detecteaza mai multe argumente
                        bool argumente_valide = valideazaArgNumePersoana(sir_argumente);
                        
                        if(argumente_valide) // daca argumentul (este doar unul singur) este valid 
                        {
                            // atunci se iau valorile corepsunzatoare argumentelor
                            std::string nume_persoana = sir_argumente.at(0);
                            
                            // si se incearca listarea informatiilor persoanei
                            // metoda 'listeazaInfoCarte(id)' listeaza informatiile doar daca id-ul exista cu adevarat
                            biblioteca->listeazaInfoPersoana(nume_persoana);
                        }
                        else
                            std::cout << "eroare: argumentele pentru comanda 'P' nu sunt valide, va rugam incercati din nou" << std::endl;
                    }
                    else
                    {
                        std::cout << "eroare: argumentele pentru comanda 'P' nu sunt valide, va rugam incercati din nou" << std::endl;
                        std::cout << "sugestie: argumentele trebuie separtate prin virgula" << std::endl;
                    }
                }
                    break;
                
                case 'S': // stop aplicatie
                    continua_rularea = false; // se opreste aplicatie
                    std::cout << "programul s-a oprit" << std::endl;
                    break;
                    
                case '\0':
                default:
                    std::cout << "eroare: linia introdusa nu contine o comanda valida, va rugam incercati din nou" << std::endl;
                    break;
            }
        }
        else
            std::cout << "eroare: nu s-a introdus nici o comanda, va rugam incercati din nou" << std::endl;

    }
    
    return 0;
}
