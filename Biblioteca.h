#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>

class Persoana; // nedefinita dar declarata de dinainte, pentru aputea creea macar pointeri de tip 'Persoana'

/* Clasa ce modeleaza conceptul de carte.
   Pentru aceasta clasa s-au luat urmatoarele decizii:
   - proprietati:
        - proprietatile clasei vor fi sub forma de variabile directe, nu pointeri catre variabile, deoarece:
            - clasa este simpla iar, in felul acesta, toate proprietatile clasei vor fi grupate in acelasi loc in memorie
            - compilatorul va sti cu usurinta unde sa gaseasca proprietatile, fara a mai avea nevoie de adrese ale
              obiectelor externe
            - pentru ca sunt alocate static, nu este nevoie sa fie dealocate manual, compilatorul se va ocupa de dealocare
    - constructori:
        - daca argumentul trebuie modificat si nu este un sir, argumentul va fi pasat prin referinta (astfel se stie
          aproape sigur ca argumentul nu este null - putini oameni dau mai departe referinta unui obiect null)
        - daca argumentul nu trebuie modificat, atunci va fi pasat prin referinta constanta in construnctor
        - daca argumentul nu trebuie modificat si este de tip primitiv (int, bool, etc.), atunci va fi pasat prin valoare
    - metode:
        - argumentele pasate prin referinta (constanta sau nu) nu se verifica daca sunt nule (desi ar putea fi nule)
    */
class Carte
{
// private:
//     int cod = 0;
//     string autor = "n/a";
//     string nume = "n/a";
//     int an_publicatie = 0;

public:
    int cod = 0;
    std::string autor = "n/a";
    std::string nume = "n/a";
    int an_publicatie = 0;
    bool este_imprumutata = false;
    Persoana* persoana_imprumutare; // este pointer si nu valoare, deoarece clasa Persoana este inca doar declarata, nu si implementata
    
    Carte(int cod, std::string& autor, std::string& nume, int an_publicatie)
    {
        if(cod >= 0)
            this->cod = cod;
        this->autor = autor;
        this->nume = nume;
        this->an_publicatie = an_publicatie;
    }

    Carte()
    {
        std::string autor = "n/a";
        std::string nume = "n/a";
        Carte(0, autor, nume, 0);
    }
    
    std::string catreString()
    {
        std::string rezultat = "";
        return rezultat + "cod: " + std::to_string(cod) + ", autor: " + autor + ", nume: " + nume +
                        ", an: " + std::to_string(an_publicatie) + ", imprumutata: " + std::to_string(este_imprumutata);
    }
};

class Persoana
{
    friend class Biblioteca;
private:
    std::string nume;
    ListaLegata<Carte> sir_carti_imprumutate;

public:
    Persoana(std::string& nume) { this->nume = nume; }
    
    Persoana() { this->nume = "n/a"; }
    
    std::string catreString()
    {
        std::string rezultat = nume + ", carti: ";
        
        // inainte de a se adauga persoana, se verifica daca nu a fost deja adaugata la lista de persoanele
        // verificarea se face dupa nume, avand in vedere ca persoanele vor fi specificate dupa nume
        IteratorInainte<Carte>* iterator = sir_carti_imprumutate.iteratorInainte();
        Carte* carte_curenta = nullptr;
        while(iterator->areUrmatorul())
        {
            // metoda 'citesteUrmatorulSiInainteaza()' returneaza (pointer catre) urmatorul element
            // si face iteratorul sa avanseze
            carte_curenta = iterator->citesteUrmatorulSiInainteaza();
            rezultat += carte_curenta->nume + ", ";
        }
        
        return rezultat;
    }
};

class Biblioteca
{
private:
    ListaLegata<Carte>* sir_carti = nullptr; // lista (un fel de deque) cu cartile disponibile
    ListaLegata<Persoana>* sir_persoane = nullptr; // lista cu persoanele care au imprumutat carti
    int numar_maxim_carti = std::numeric_limits<int>::max();; // nr. maxim de carti permise in biblioteca

public:
    Biblioteca(int nr_maxim_carti)
    {
        sir_carti = new ListaLegata<Carte>();
        sir_persoane = new ListaLegata<Persoana>();
        
        if(nr_maxim_carti >= 1)
            this->numar_maxim_carti = nr_maxim_carti;
    }
    
    void adauga(Carte* carte)
    {
        if(carte != nullptr)
        {
            if(sir_carti->marime() + 1 <= numar_maxim_carti)
                sir_carti->adauga(*carte);
            else
               std::cout << "ne pare rau, s-a atins numarul maxim de carti" << std::endl; 
        }
        else
            std::cout << "void adauga(Carte* carte): eroare: s-a pasat un argument null" << std::endl;
    }
    
    void listeazaCarti()
    {
        IteratorInainte<Carte>* iterator = sir_carti->iteratorInainte();
        
        if(sir_carti->marime())
        {
            while(iterator->areUrmatorul())
            {
                std::cout << iterator->citesteUrmatorulSiInainteaza()->catreString() << std::endl;
            }
        }
        else
            std::cout << "momentan nu exista carti in bibblioteca" << std::endl;
    }
    
    bool eliminaCarte(int id)
    {
        IteratorInainte<Carte>* iterator = sir_carti->iteratorInainte();
        
        bool carte_negasita = true; // pentru oprirea buclei while imediat dupa ce s-a gasit cartea
        Carte* carte_curenta = nullptr; // cartea de la iteratia curenta
        while(iterator->areUrmatorul() && carte_negasita)
        {
            // se citeste cartea curenta, fara ca iteratorul sa avanseze la elementul urmatorul
            // (adica metoda 'citesteUrmatorul()' doar citeste urmatorul element, nu face iteratorul sa avanseze)
            carte_curenta = iterator->citesteUrmatorul();
            
            // observatie: nu are logica sa se elimine din lista o carte imprumutata (adica nereturnata)
            // daca s-a gasit cartea si cartea nu este imprumutata
            if(carte_curenta->cod == id && carte_curenta->este_imprumutata == false)
            {
                iterator->scoateUrmatorulSiInainteaza(); // se scoate urmatorul element si se avanseaza iteratorul
                carte_negasita = false; // se opreste bucla while
            }
            else
                 // este necesar, altfel nu avanseaza iteratorul la urmatorul element (pe ramura 'else')
                iterator->citesteUrmatorulSiInainteaza();
        }
        
        if(carte_negasita)
            return false; // nu s-a gasit si nu s-a eliminat cartea
        else
            return true; // cartea a fost gasita si s-a eliminat
    }
    
    bool imprumutaCarte(int id_carte, std::string& nume_persoana)
    {
        bool carte_imprumutata = false; // valoarea returnata de metoda
        
        // prima data, se cauta cartea (daca nu se gaseste, se returneaza nullptr)
        Carte* carte = cautaCarte(id_carte);
        
        // daca s-a gasit cartea
        if(carte != nullptr)
        {
            // daca nu s-a imprumutat deja cartea
            if(carte->este_imprumutata == false)
            {
                // cand se imprumuta o carte unei persoane, in mod normal se creeaza o noua persoana
                // dar poate acea persoana exista deja; de aceea, mai intai se cauta persoana, dupa nume
                // metoda cautaPersoana(nume_persoana) returneaza un obiect de tip Persoana daca persoana a fost gasita
                // altfel returneaza nullptr
                
                Persoana* persoana = cautaPersoana(nume_persoana);
                // daca persoana exista deja
                if(persoana != nullptr)
                {
                    // atunci se atribuie persoana cartii
                    carte->persoana_imprumutare = persoana;
                    // si se adauga cartea in lista carti imprumutate a pesoanei respective
                    persoana->sir_carti_imprumutate.adauga(*carte);
                }
                else
                {
                    // altfel, se creeaza o noua persoana cu acel nume, si se adauga in sirul de persoane
                    Persoana* persoana_noua = new Persoana(nume_persoana);
                    // o carte stie carei persoane a fost imprumutata, deci se atribuie persoana cartii
                    carte->persoana_imprumutare = persoana_noua;
                    // se adauga cartea in lista carti imprumutate a pesoanei respective
                    persoana_noua->sir_carti_imprumutate.adauga(*carte);
                    sir_persoane->adauga(*persoana_noua);
                }
                carte->este_imprumutata = true;
                
                carte_imprumutata = true; // cartea s-a imprumutat cu succes
            }
        }

        return carte_imprumutata; // nu s-a gasit si nu s-a imprumutat cartea 
    }
    
    Carte* cautaCarte(int id)
    {
        IteratorInainte<Carte>* iterator = sir_carti->iteratorInainte();
        
        bool carte_negasita = true; // pentru oprirea buclei while imediat dupa ce s-a gasit cartea
        
        Carte* carte_curenta = nullptr; // cartea de la iteratia curenta
        while(iterator->areUrmatorul() && carte_negasita)
        {
            // se citeste cartea curenta, fara ca iteratorul sa avanseze la elementul urmatorul
            // (adica metoda 'citesteUrmatorul()' doar citeste urmatorul element, nu face iteratorul sa avanseze)
            carte_curenta = iterator->citesteUrmatorulSiInainteaza();
            if(carte_curenta->cod == id)
                carte_negasita = false; // se opreste bucla while
        }
        
        if(carte_negasita)
            return nullptr;
        else
            return carte_curenta;
    }
    
    void listeazaInfoCarte(int id)
    {
        Carte* carte = cautaCarte(id); // mai intai, se cauta cartea in sirul de carti
        
        if(carte != nullptr) // daca s-a gasit cartea
            std::cout << "info carte id " << carte->cod << ": autor: " << carte->autor
                        << ", titlu: " << carte->nume << ", an publicatie: " << carte->an_publicatie 
                        << ", imprumutata: " << carte->este_imprumutata << std::endl;
        else
            std::cout << "cartea cu id " << id << " nu a fost gasita" << std::endl;
    }
    
    // metoda mai mult pentru uz intern; cauta o persoana dupa nume
    // returneaza 'nullptr' daca nu gaseste persoana
    Persoana* cautaPersoana(std::string& nume)
    {
        bool persoana_negasita = true;
        
        // inainte de a se adauga persoana, se verifica daca nu a fost deja adaugata la lista de persoanele
        // verificarea se face dupa nume, avand in vedere ca persoanele vor fi specificate dupa nume
        IteratorInainte<Persoana>* iterator = sir_persoane->iteratorInainte();
        Persoana* persoana_curenta = nullptr;
        while(iterator->areUrmatorul() && persoana_negasita)
        {
            // metoda 'citesteUrmatorulSiInainteaza()' returneaza (pointer catre) urmatorul element
            // si face iteratorul sa avanseze
            persoana_curenta = iterator->citesteUrmatorulSiInainteaza();
            if(persoana_curenta->nume == nume)
                persoana_negasita = false; // se opreste iteratia
        }
        
        if(persoana_negasita)
            return nullptr;
        else
            return persoana_curenta;
    }
    
    void listeazaInfoPersoana(std::string& nume)
    {
        Persoana* persoana = cautaPersoana(nume); // mai intai, se cauta persoana in sirul de persoane
        
        // daca persoana a fost gasita
        if(persoana != nullptr)
        {
            std::cout << "info persoana " << persoana->nume << ": carti imprumutate: " << std::endl;
            
            // cartile imprumutate de fiecare persoana sunt stocate in persoana in sine, in sirul
            // 'sir_carti_imprumutate' de tip ListaLegata<Carte> (o clasa personalizata)
            // se printeaza, pe rand, toate cartile imprumutate de persoana
            // pentru iterare se foloseste o clasa personalizata, 'IteratorInainte<Carte>'
            Carte* carte_curenta = nullptr;
            IteratorInainte<Carte>* iterator = persoana->sir_carti_imprumutate.iteratorInainte();
            while(iterator->areUrmatorul())
            {
                carte_curenta = iterator->citesteUrmatorulSiInainteaza();
                std::cout << "    " << carte_curenta->catreString() << std::endl;
            }
        }
        
        else // daca persoana nu a fost gasita
            std::cout << "persoana cu nume " << nume << " nu a fost gasita" << std::endl;
    }
};

#endif // BIBLIOTECA_H
