#include "operation.hpp"
#include <algorithm>
#include "graf.hpp"
#include <stdio.h>
#include <cstring>

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


int penaltyCounter(const std::vector<Job*> &pi) {
    int size = pi.size();
    int *C = new int[size];
    int *T = new int[size];
    int wiTi = 0;

    for(int i = 0; i < size; ++i) {
        if(i) C[i] = C[i-1] + pi[i]->processingTime;
        else C[i] = pi[i]->processingTime;

        T[i] = std::max(C[i] - pi[i]->deadline, 0);
        wiTi += T[i] * pi[i]->weight;
    }

    delete[] C;
    delete[] T;
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
        int tmp = penaltyCounter(combinations[i]); // policz Cmax
        if(tmp < wiTi) { // jeżeli okaże się najlepsza
            wiTi = tmp; // zapisz Cmaks
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
    

    

    // printf("%d %d", memory[0], memory[N.size()-1]);


}
