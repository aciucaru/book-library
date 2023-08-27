#ifndef LISTALEGATA_H_INCLUDED
#define LISTALEGATA_H_INCLUDED

#include <iostream>
#include <vector>
#include <string>
#include <limits>

template<typename E> class IteratorInainte;
template<typename E> class IteratorInapoi;

// clasa ce modeleaza elementul unei cozi duble
template<typename E> class ElementListaLegata
{
    // prieteni, pentru a putea accesa variabile private fara getteri
    template<typename T> friend class ListaLegata;
    template<typename T> friend class IteratorInainte;
    template<typename T> friend class IteratorInapoi;
    
private:
    // pentru ca este vorba de o lista legata dubla, elementul va avea pointer catre elementul urmator dar si cel precedent
    E element; // elementul efectiv stocat in aceasta clasa
    ElementListaLegata<E>* precedentul = nullptr; // pointer catre elementul precedent din lista legata
    ElementListaLegata<E>* urmatorul = nullptr; // pointer catre elementul urmator din lista legata
    
public:
    /* constructorul principal, acest constructor primeste ca argument elementul pe care instanta clasei ar trebui
       sa il reprezinte*/
    ElementListaLegata(E& element) { this->element = element; }
};


// clasa ce modeleaza un sir dinamic (redimensionabil) de tip lista legata dubla
// elementele acestei clase vor fi de tip ElementListaLegata<E>
template<typename E> class ListaLegata
{
    // prieteni, pentru a putea accesa variabile private fara getteri
    template<typename T> friend class IteratorInainte;
    template<typename T> friend class IteratorInapoi;
    friend class TestareListaLegata;
    
private:
    ElementListaLegata<E>* primul_element = nullptr; // pointer catre primul element al listei legate
    ElementListaLegata<E>* ultimul_element = nullptr; // pointer catre ultimul element al listei legate
    int lungime = 0; // numarul de elemente din lista legata
    
public:
    ListaLegata() { }
    
    // ************ metode ce returneaza dar nu modifica starea sau elementele listei legate ***********************

    // metoda ce returneaza primul element sub forma completa de nod (adica ElementListaLegata<E>* in loc de E*)
    // este folositoare pentru uz intern, cand se doreste tot nodul, nu doar valoarea din interiorul nodului
    // metoda doar returneaza, nu sterge nodul din lista
    ElementListaLegata<E>* primulNod()
    {
        if(primul_element != nullptr)
            return primul_element;
        else
            return nullptr;
    }
    
    // metoda ce returneaza ultimul element sub forma completa de nod (adica ElementListaLegata<E>* in loc de E*)
    // metoda doar returneaza, nu sterge nodul din lista
    ElementListaLegata<E>* ultimulNod()
    {
        if(ultimul_element != nullptr)
            return ultimul_element->element;
        else
            return nullptr;
    }
    
    // metoda ce returneaza elementul (valoarea) din primul nod al listei legate
    // metoda doar returneaza, nu sterge din lista
    E* primul()
    {
        if(primul_element != nullptr)
            return &(primul_element->element); // se returneaza adresa propretatii 'element'
        else
            return nullptr;
    }

    // metoda ce returneaza elementul (valoarea) din ultimul nod al listei legate
    // metoda doar returneaza, nu sterge din lista
    E* ultimul() // metoda ce returneaza ce returneaza pointer catre ultimul element al listei legate
    {
        if(ultimul_element != nullptr)
            return &(ultimul_element->element); // se returneaza adresa propretatii 'element'
        else
            return nullptr;
    }

    int marime() { return lungime; } // metoda ce returneaza marimea listei legate

    
    // ************ metode ce modifica starea si elementele listei legate ********************************

    void adauga(E& element) // metoda ce adauga un element la sfarsitul listei legate
    {
        // daca inca nu s-a adaugat nici un element la lista legata
        if(lungime == 0)
        {
            // atunci inseamna ca primul element este inca null, deci acela primeste elementul adaugat
            ElementListaLegata<E>* element_nou = new ElementListaLegata<E>(element);
            primul_element = element_nou; // elementul nou adaugat devine primul element al listei legate
            ultimul_element = element_nou; // elementul nou adaugat devine si ultimul element al listei legate
        }
        // iar daca s-a adaugat exact un element la lista legata
        else if(lungime == 1)
        {
            // atunci inseamna ca primul element este deja ocupat, deci se adauga argumentul la ultimul element
            ElementListaLegata<E>* element_nou = new ElementListaLegata<E>(element);
            primul_element->urmatorul = element_nou; // primul element 'va arata' catre ultimul element
            ultimul_element = element_nou;
            ultimul_element->precedentul = primul_element; // ultimul element 'va arata' catre primul element
        }
        // iar daca s-au adaugat mai mult de 1 element la lista legata
        else if(lungime > 1)
        {
            ElementListaLegata<E>* element_nou = new ElementListaLegata<E>(element);
            ultimul_element->urmatorul = element_nou;
            element_nou->precedentul = ultimul_element;
            ultimul_element = element_nou;
        }
        lungime++; // pentru ca s-a adaugat un nou element, lungimea listei legate a crescut cu un element
    }

    // metoda ce elimina si returneaza (pointer catre) valoarea primului nod al listei legate
    E* scoatePrimul()
    {
        E* element_de_returnat = nullptr;
        // daca inca nu exista nici un element in lista legata
        if(lungime == 0)
            element_de_returnat = nullptr;
        // daca exista un singur element in lista legata
        else if(lungime == 1)
        {
            ElementListaLegata<E>* element_temp = primul_element;
            primul_element = nullptr;
            ultimul_element = nullptr;
            lungime = 0;
            
            element_de_returnat = &(element_temp->element);
        }
        // daca exista minim 2 elemente in lista legata
        else if(lungime >= 2)
        {
            ElementListaLegata<E>* al_doilea_element = primul_element->urmatorul;
            ElementListaLegata<E>* element_temp = primul_element;
            
            primul_element->urmatorul = nullptr;
            al_doilea_element->precedentul = nullptr;
            primul_element = al_doilea_element;
            lungime--;
            
            element_de_returnat = &(element_temp->element);
        }
        return element_de_returnat;
    }

    // metoda ce elimina si returneaza (pointer catre) valoarea ultimului nod al listei legate
    E* scoateUltimul()
    {
        E* element_de_returnat = nullptr;
        // daca inca nu exista nici un element in lista legata
        if(lungime == 0)
            element_de_returnat = nullptr;
        // daca exista un singur element in lista legata
        else if(lungime == 1)
        {
            ElementListaLegata<E>* element_temp = ultimul_element;
            primul_element = nullptr;
            ultimul_element = nullptr;
            lungime = 0;
            
            element_de_returnat = &(element_temp->element);
        }
        // daca exista minim 2 elemente in lista legata
        else if(lungime >= 2)
        {
            ElementListaLegata<E>* element_temp = ultimul_element;
            ElementListaLegata<E>* penultim_element = ultimul_element->precedentul;
            penultim_element->urmatorul = nullptr;
            ultimul_element->precedentul = nullptr;
            ultimul_element = penultim_element;
            lungime--;
            
            element_de_returnat = &(element_temp->element);
        }
        
        return element_de_returnat;
    }
    
    // ************************ metode pentru iterare ********************************
    
    // metoda ce returneaza (pointer catre) un iterator cu iterare in sens pozitiv (inainte)
    // iteratorul primeste aceasta lista legata ca argument si are acces la elementele listei
    // pentru fiecare iterare noua ar trebui utilizat un iterator nou
    IteratorInainte<E>* iteratorInainte() { return new IteratorInainte<E>(this); }
};


/* - clasa ce modeleaza conceptul de iterator; clasa este folosita pentru a putea itera lista legata
   - iteratorul este generat si returnat de lista legata
   - pentru fiecare iterare (bucla) in parte, se recomanda creearea unui nou iterator; in acest fel, cea ce s-a
     iterat cu un iterator nu va infuenta alta iterare, daca se foloseste alt iterator
   - iteratorul doar are acces la lista legata si doar itereaza prin elemente, el nu copiaza elementele listei legate
   - acest iterator este pentru iterarea in sens pozitiv-inainte (de la primul la ultimul element) */
template<typename E> class IteratorInainte
{
private:
    ListaLegata<E>* lista_legata;
    ElementListaLegata<E>* element_curent = nullptr; // pointer catre elementul curent, ce urmeaza a fi returnat
    bool are_urmatorul_element = false;
    bool iterare_inceputa = false; // daca iterarea a inceput sau nu
    int numar_modificari = 0;
    
//     int contor_debug1 = 0;
//     int contor_debug2 = 0;
    
public:
    IteratorInainte(ListaLegata<E>* lista_legata)
    {
        this->lista_legata = lista_legata;
        
        // constructorul este reponsabil, o singura data, la inceput, sa stabileasca daca lista legata
        // are sau nu un element urmator (numit aici 'element_curent')
        if(lista_legata->marime() >= 1)
            element_curent = lista_legata->primulNod();
    }
    
    // metoda ce spune daca, in timpul iteratiei, aceasta lista legata mai are inca un element (DUPA cel curent)
    // se foloseste pentru iterare intr-o bucla while
    bool areUrmatorul()
    {
        if(element_curent != nullptr)
            return true;
        else
            return false;
    }
    
    // metoda ce returneaza (fara sa elimine si fara sa avanseze), in timpul iteratiei, urmatorul element
    // se foloseste in corelatie cu metoda 'areUrmatorul()'
    E* citesteUrmatorul()
    {
        if(areUrmatorul())
            return &(element_curent->element);
        else
            return nullptr;
    }
    
    // metoda ce returneaza (fara sa elimine), in timpul iteratiei, urmatorul element
    // metoda avanseaza catre urmatorul element din lista legata
    // se foloseste in corelatie cu metoda 'areUrmatorul()'
    E* citesteUrmatorulSiInainteaza()
    {
        E* element_de_returnat = nullptr;
        
        if(areUrmatorul())
        {
            // se initializeaza elementul de returnat
            // in acest caz, 'urmatorul()' se refera de fapt la 'element_curent'
            element_de_returnat = &(element_curent->element);
            
            /* iar apoi se schimba elementul curent, facand-ul sa arate catre elementul urmator;
             * acest lucru se face indiferent daca propr. 'urmatorul' al elementului curent este nula sau nu;
             * daca propr. 'urmatorul' nu este nula, atunci metoda areUrmatorul() va returna adevarat iar iterarea
             * va continua, daca propr. 'urmatorul' este nula, atunci si metoda 'areUrmatorul()' va returna fals
             * iar iterarea se va opri*/
            element_curent = element_curent->urmatorul;
        }
        
        return element_de_returnat;
    }

    // metoda ce returneaza (si ELIMINA), in timpul iteratiei, urmatorul element
    // se foloseste in corelatie cu metoda 'areUrmatorul()'
    E* scoateUrmatorulSiInainteaza()
    {
        E* element_de_returnat = nullptr;
        
        if(areUrmatorul()) // metoda 'areUrmatorul()' se refera la existenta obiectului 'element_curent'
        {
            // se initializeaza elementul de returnat
            // in acest caz, 'urmatorul()' se refera de fapt la 'element_curent'
            element_de_returnat = &(element_curent->element);
            lista_legata->lungime--;
            
            /* daca se scoate un element din lista legata, atunci trebuie conectate elementul dinaintea
             * si elementul dupa elementul ce sa va scoate; exista posibilitatea sa nu existe ambele elemente */
            
            ElementListaLegata<E>* element_precedent = element_curent->precedentul;
            ElementListaLegata<E>* element_urmator = element_curent->urmatorul;
            // exista 4 cazuri posibile:
            // cazul 1: elementul de scos are si element precedent si element urmator
            if(element_precedent != nullptr && element_urmator != nullptr)
            {
                // in mod normal, 'element_precedent->urmatorul' arata catre 'element_curent', ce trebuie scos,
                // iar acum va arata catre 'element_urmator'
                element_precedent->urmatorul = element_urmator;
                
                // in mod normal, 'element_urmator->precedentul' arata catre 'element_curent', ce trebuie scos,
                // iar acum va arata catre 'element_precedent'
                element_urmator->precedentul = element_precedent;
                
                /* se sterg legaturile elementului curent cu vecinii sai; aceste operatii sunt optionale, caci
                 * pointerul 'element_curent' oricum va arata catre urmatorul element la sfasitul metodei,
                 * dar este recomandat pentru siguranta */
                element_curent->precedentul = nullptr;
                element_curent->urmatorul = nullptr;
                
                // dupa conectare elementului precedent cu cel urmator, se reactualizeaza si elementul curent
                element_curent = element_urmator; // conform ramurii if, 'element_urmator' este garantat nenul
            }
            
            // cazul 2: elementul de scos are element precedent dar nu si element urmator
            // adica elementul de scos este ultimul element al listei legate
            if(element_precedent != nullptr && element_urmator == nullptr)
            {
                // in mod normal, 'element_precedent->urmatorul' arata catre 'element_curent', ce trebuie scos,
                // iar acum va arata catre 'element_urmator'
                element_precedent->urmatorul = nullptr;
                
                /* se sterge legatura elementului curent cu vecinul sau (precedentul)
                 * aceasta operatie este optionala */
                element_curent->precedentul = nullptr;
                
                // dupa conectare elementului precedent cu cel urmator, se reactulizeaza si elementul curent
                element_curent = nullptr;  // conform ramurii if, 'element_urmator' este garantat null
                
                // trebuie actualizate si lista legata, altfel modificare nu va fi vizibila in lista
                lista_legata->ultimul_element = element_precedent;
            }
            
            // cazul 3: elementul de scos nu are element precedent dar are element urmator
            // adica elementul de scos este primul element al listei legate
            if(element_precedent == nullptr && element_urmator != nullptr)
            {
                // in mod normal, 'element_urmator->precedentul' arata catre 'element_curent', ce trebuie scos,
                // iar acum va arata catre 'element_precedent'
                element_urmator->precedentul = nullptr;
                
                /* se sterge legatura elementului curent cu vecinul sau (urmatorul)
                 * aceasta operatie este optionala */
                element_curent->urmatorul = nullptr;
                
                // dupa conectare elementului precedent cu cel urmator, se reactulizeaza si elementul curent
                element_curent = element_urmator; // conform ramurii if, 'element_urmator' este garantat nenul
                
                // trebuie actualizate si lista legata, altfel modificare nu va fi vizibila in lista
                lista_legata->primul_element = element_urmator;
            }
            
            // cazul 4: elementul de scos nu are nici element precedent si nici element urmator
            // adica elementul de scos este singurul element al listei legate
            if(element_precedent == nullptr && element_urmator == nullptr)
                element_curent = nullptr; // conform ramurii if, 'element_urmator' este garantat nul
        }

        return element_de_returnat;
    }
};

#endif // LISTALEGATA_H_INCLUDED
