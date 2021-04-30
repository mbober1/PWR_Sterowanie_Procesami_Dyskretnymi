#include "operation.hpp"
#include <algorithm>
#include "graf.hpp"
#include <stdio.h>
#include <cstring>
#include <bitset>
#include <iostream>


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
 * Algorytm dynamiczny.
 *
 * @param N Wektor operacji.
 * @return Zwraca najlepszą kombinację.
 */
std::vector<Job*> Dynamic(std::vector<Job*> N) {
    int size = N.size();
    int memSize = 1<<size;
    int memory[memSize];
    
    for (size_t i = 0; i < memSize; i++) memory[i] = -1;

    auto g = Graf<Job*>::tree(N); // buduj drzewo // 21s
    auto combinations = traverse(g); // generuj kombinacje // 40s

    for(int i = 0; i < combinations.size(); ++i) { // dla każdej kombinacji
        int combinationSize = combinations[i].size(); // tyle jest zadań w kombinacji
        std::bitset<8> tmp(0); 

        for (size_t j = 0; j < combinationSize; j++) 
        {
            int dupa = combinations[i][j]->number - 1;
            tmp |= 1<<(dupa);
            // std::cout << dupa << "|" << tmp << std::endl;
        }

        unsigned long idx = tmp.to_ulong();
        // std::cout << tmp << "=" << idx << std::endl;

        if(memory[idx] == -1) {
            printf("policz %d\n", idx);
            memory[idx] = 7;
        } else {
            // printf("%d już policzone\n", idx);
        }
    }
    
    return N;

}
