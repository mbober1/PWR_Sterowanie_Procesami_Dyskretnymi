#include <vector>
#include "operation.hpp"
#include "RandomNumberGenerator.h"
#include "graf.hpp"
#include <stdio.h>


/**
 * Wylicza czas wykonywania danej kombinacji.
 *
 * @param N Wektor operacji.
 * @return Zwraca czas wykonywania danej kombinacji.
 */
int Cemaks(const std::vector<Job*> &N) {
    int Cmax1 = 0;
    int Cmax2 = 0;

    for(int j = 0; j < N.size(); ++j) {
        Cmax1 += N[j]->op[0].duration;
        Cmax2 = std::max(Cmax1, Cmax2)  + N[j]->op[1].duration;
    }
    return Cmax2;
}


/**
 * Szukanie najmniejszego czasu wykonywania.
 *
 * @param N Wektor operacji.
 * @param it Iterator najlepszej operacji.
 * @return Zwraca operację z najkrótszym czasem wykonywania.
 */
Job minP(std::vector<Job> &N, int *it) {
    Job minJob = N[0];
    *it = 0;

    for(int i = 0; i < N.size(); ++i) {
        if(N[i].op[0].duration < minJob.op[0].duration && N[i].op[0].duration < minJob.op[1].duration) {
            minJob = N[i];
            *it = i;
        } 
        if(N[i].op[1].duration < minJob.op[0].duration && N[i].op[1].duration < minJob.op[1].duration) {
            minJob = N[i];
            *it = i;
        }
    }
    return minJob;
}


/**
 * Algorytm Jonson.
 *
 * @param N Wektor operacji.
 * @return Zwraca najlepszą kombinację.
 */
std::vector<Job> Jonson(std::vector<Job> N) {
    int l = 0;
    int k = N.size() - 1;
    std::vector<Job> Pi;
    Pi.resize(N.size());

    while(!N.empty()) {
        int it;
        Job minJob = minP(N, &it);
        if(minJob.op[0].duration < minJob.op[1].duration) {
            Pi.at(l++) = minJob;
        } else {
            Pi.at(k--) = minJob;
        }
        N.erase(N.begin() + it);
    }

    return Pi;
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
 * Algorytm przeszukiwaina siłowego.
 *
 * @param N Wektor operacji.
 * @return Zwraca najlepszą kombinację.
 */
std::vector<Job*> BruteForce(std::vector<Job*> N) {
    int minCmax = (1<<16); // minimalny Cmax
    std::vector<Job*>* Pi; // najlepsza kombinacja

    auto g = Graf<Job*>::tree(N); // buduj drzewo
    auto combinations = traverse(g); // generuj kombinacje

    for(int i = 0; i < combinations.size(); ++i) { // dla każdej kombinacji
        int x = Cemaks(combinations[i]); // policz Cmax
        if(x < minCmax) { // jeżeli okaże się najlepsza
            minCmax = x; // zapisz Cmaks
            Pi = &(combinations[i]); // i najlepszą kombinację
        }
    }
    return *Pi;
}