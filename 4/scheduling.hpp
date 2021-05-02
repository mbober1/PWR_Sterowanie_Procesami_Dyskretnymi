#include "operation.hpp"
#include <algorithm>
#include "graf.hpp"
#include <stdio.h>
#include <cstring>
#include <bitset>
#include <iostream>
#include <map>


/**
 * Wybierz najmniejszy deadline.
 *
 * @param i Zadanie pierwsze.
 * @param i Zadanie drugie.
 * @return Zwraca TRUE gdy zadanie pierwsze ma mniejszy deadline.
 */
inline bool minDead(const Job* i, const Job* j) { return(i->deadline < j->deadline); }


/**
 * Algorytm Greedy.
 *
 * @param N Wektor operacji.
 * @return Zwraca najlepszą kombinację.
 */
std::vector<Job*> Greedy(std::vector<Job*> N) {
    std::sort(N.begin(), N.end(), minDead);
    return N;
}


/**
 * Przeszukuje graf rekurencyjnie.
 *
 * @param graph Graf z danymi.
 * @param values Wektor pozostałych kombinacji.
 * @param node Numer aktualnego wierzchołka.
 * @return Zwraca wszystkie możliwe kombinacje.
 */
std::vector<std::vector<Job*>>traverse(Graf<Job*> &graph, const std::vector<std::vector<Job*>> values = {{}}, int node = 0) {
    if (graph.adj[node].size() > 1) { // jeżeli wierzchołek ma więcej jak jednego sąsiada
        auto res = std::vector<std::vector<Job*>>(); // wektor możliwych kombinacji

        for (auto i = graph.adj[node].begin(); i != graph.adj[node].end(); ++i) { // dla każdego dziecka
            auto x = traverse(graph, values, *i); // wywołaj się rekurencyjnie
            res.insert(res.end(), x.begin(), x.end()); // dołącz do możliwych kombinacji 
        }
        if (node != 0) { // jeśli nie jesteśmy zerem (korzeniem)
            for (int i = 0; i < res.size(); ++i) { // przejdż po każdej kombinacji
                    res[i].push_back(graph.vertices[node]); // i dołącz aktualny wierzchołek
            }
        }

        return res;
    } else { // gdy wierzchołek ma jednego sąsiada
        Job* child = graph.vertices[graph.adj[node][0]]; // liść
        Job* parent = graph.vertices[node]; // przedostatni element
        return {{child, parent}};
    }
}


/**
 * Policz karę dla podanej kombinacji.
 *
 * @param pi Wektor operacji.
 * @return Zwraca najlepszą kombinację.
 */
int penaltyCounter(const std::vector<Job*> &pi) {
    int size = pi.size(); // ilość zadań
    int *C = new int[size]; // tablica z czasami zakończenia
    int *T = new int[size]; // tablica z opóźnieniami
    int wiTi = 0; // kara ostateczna

    for(int i = 0; i < size; ++i) { // dla każego zadania
        if(i) C[i] = C[i-1] + pi[i]->processingTime; // policz czas zakończenia
        else C[i] = pi[i]->processingTime; // policz czas zakończenia 

        T[i] = std::max(C[i] - pi[i]->deadline, 0); // policz spóżnienia
        wiTi += T[i] * pi[i]->weight; // dodaj do ostatecznego wyniku
    }

    delete[] C; // usuwanko
    delete[] T; // usuwanko
    return wiTi;
}


/**
 * Algorytm przeszukiwaina siłowego.
 *
 * @param N Wektor operacji.
 * @return Zwraca najlepszą kombinację.
 */
std::vector<Job*> BruteForce(std::vector<Job*> N) {
    int wiTi = (1<<16); // minimalny Cmax
    std::vector<Job*>* Pi; // najlepsza kombinacja

    auto g = Graf<Job*>::tree(N); // buduj drzewo
    auto combinations = traverse(g); // generuj kombinacje

    for(int i = 0; i < combinations.size(); ++i) { // dla każdej kombinacji
        int tmp = penaltyCounter(combinations[i]); // policz karę
        if(tmp < wiTi) { // jeżeli okaże się najmniejsza
            wiTi = tmp; // zapisz karę
            Pi = &(combinations[i]); // i najlepszą kombinację
        }
    }
    return *Pi;
}


/**
 * Policz sumę czasów działania.
 *
 * @param D Wektor operacji.
 * @return Zwraca sumę czasów działania.
 */
int processingSum(const std::vector<Job*> &D) {
    int sum = 0; // suma czasów

    for (size_t i = 0; i < D.size(); i++) // dla każdej operacji
    {
        sum += D[i]->processingTime; // dodaj czas
    }
    
    return sum;
}


/**
 * Stwórz wektor operacji na podstawie bitów.
 *
 * @param D Wektor wszystkich operacji.
 * @return Zwraca wektor wybranych operacji.
 */
std::vector<Job*> createD(std::vector<Job*> N, int i) {
    std::vector<Job*> tmp; // nowy wektor operacji

    for (size_t j = 0; j < N.size(); j++) // dla każdej operacji
    {
        if(i & (1<<j)) tmp.push_back(N[j]); // dodaj do nowego wektora gdy spełnia bitset
    }

    return tmp;
}


/**
 * Algorytm dynamiczny.
 *
 * @param N Wektor operacji.
 * @return Zwraca najlepszą kombinację.
 */
std::vector<Job*> Dynamic(std::vector<Job*> N) {
    int size = N.size(); // ilość operacji
    std::vector<int> memory(1<<size, 0); // tablica wyników
    std::map<int, Job*> results; // mapa rezultatów
    results[0] = nullptr;


    for (size_t i = 1; i < 1<<size; i++) // dla każdej operacji
    {
        auto D = createD(N, i); // stwórz nowy wektor na podstawie bitsetu
        int sum = processingSum(D); // policz sume czasów wykonywania
        int min = 1<<16; // minimum
        int minJ; // indeks najmniejszego

        for (size_t j = 0; j < D.size(); j++) // dla każdej operacji w nowym wektorze
        {
            int max = std::max(sum - D[j]->deadline, 0); // policz sumę bez ostatniego deadlinea
            int idx = i ^ (1<< D[j]->number-1 ); // indeks elementu w pamięci
            int tmp = max * D[j]->weight + memory[idx]; // weź sume, pomnóż przez kare i dodaj poprzedni

            if(tmp < min) { // jeżeli okaże się mniejsze od najmniejszego
                min = tmp; // przypisz do najmniejszego
                minJ = j; // nie zapomnij o indeksie
            }
        }

        memory[i] = min; // wpisz najmniejszą wartość do pamięci
        results[i] = D[minJ]; // i dodaj do listy rezultatów
        
    }

    std::vector<Job*> J; // wektor wynikowy
    int tmp = results.size() - 1; // indeks na ostatni element

    while(tmp > 0) // gdy tmp większe od zera
    {
        J.push_back(results[tmp]); // dodaj do wyników
        tmp ^= (1<<(results[tmp]->number - 1)); // ustaw indeks na kolejny element (backtracing)
    }

    std::reverse(J.begin(), J.end()); // odwróć kolejność

    return J;
}
