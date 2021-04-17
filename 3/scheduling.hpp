#include <vector>
#include "operation.hpp"
#include "RandomNumberGenerator.h"
#include "graf.hpp"
#include <stdio.h>


int Cemaks(const std::vector<Job*> &Pi) {
    int Cmax1 = 0;
    int Cmax2 = 0;
    // printf("C: ");

    for(int j = 0; j < Pi.size(); ++j) {
        Cmax1 += Pi[j]->op[0].duration;
        Cmax2 = std::max(Cmax1, Cmax2)  + Pi[j]->op[1].duration;
        // printf("[%d, %d] ", Cmax1, Cmax2);
    }
    // printf("\n");
    return Cmax2;
}


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

std::vector<Job> jonson(std::vector<Job> N) {
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
        fflush(stdout);
        N.erase(N.begin() + it);
    }

    return Pi;
}   


// node - numer aktualnego wierzchołka
// g - drzewo
// values - wektor pozostałych kombinacji
std::vector<std::vector<Job*>>traverse(Graf<Job*> &g, std::vector<std::vector<Job*>> values = {{}}, int node = 0) {
  if (g.adj[node].size() > 1) { // jeżeli wierzchołek ma więcej jak jednego sąsiada
    auto res = std::vector<std::vector<Job*>>(); // wektor możliwych kombinacji

    for (auto i = g.adj[node].begin(); i != g.adj[node].end(); ++i) { // dla każdego dziecka
      auto x = traverse(g, values, *i); // wywołaj się rekurencyjnie
      res.insert(res.end(), x.begin(), x.end()); // dołącz do możliwych kombinacji 
    }
    if (node != 0) { // jeśli nie jesteśmy zerem (korzeniem)
      for (int i = 0; i < res.size(); ++i) { // przejdż po każdej kombinacji
        res[i].push_back(g.vertices[node]); // i dołącz aktualny wierzchołek
      }
    }
    return res;
  } else { // gdy wierzchołek ma jednego sąsiada
    Job* child = g.vertices[g.adj[node][0]]; // liść
    Job* parent = g.vertices[node]; // przedostatni element
    return {{child, parent}};
  }
}

std::vector<Job*> BruteForce(std::vector<std::vector<Job*>> combinations) {
    int minCmax = (1<<16); // minimalny Cmax
    std::vector<Job*>* C; // najlepsza kombinacja

    for(int i = 0; i < combinations.size(); ++i) { // dla każdej kombinacji
        int x = Cemaks(combinations[i]); // policz Cmax
        if(x < minCmax) { // jeżeli okaże się najlepsza
            minCmax = x; // zapis Cmaks
            C = &(combinations[i]); // najlepszą kombinację
        }
    }
    return *C;
}