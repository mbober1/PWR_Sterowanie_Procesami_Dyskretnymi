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
    int lastMachine = N[0]->last(); // indeks ostatniej maszyny
    int lastJob = N.size() - 1; // indeks ostatniego zadania

    for (int i = 0; i <= lastMachine; ++i) { // dla każdej maszyny
        for (int j = 0; j <= lastJob; ++j) { // dla każdego zadania
            int Cmax;

            if(i == 0) { // jeżeli to jest pierwsza maszyna
                if(j == 0) Cmax = 0 + N[j]->op[i].duration; // jeżeli to jest pierwsze zadanie to Cmax ustawiamy na jego czas trwania
                else Cmax = N[j-1]->op[i].end + N[j]->op[i].duration; // jeśli nie to Cmax to czas jego trwania plus koniec trwania poprzedniego

            } else { // jeżeli to nie jest pierwsza maszyna
                int prevCmax, curentMachineCmax;

                if(i == 0) prevCmax = 0; // jeżeli to jest pierwsza maszyna to prevCmax ustaw na 0
                else prevCmax = N[j]->op[i-1].end; // jeśli nie to prevCmax ustaw na koniec tego zadania w poprzedniej maszynie

                if(j == 0) curentMachineCmax = 0; // jeżeli to jest pierwsze zadanie to ustaw Cmax na obecnej maszynie na 0
                else curentMachineCmax = N[j-1]->op[i].end; // jeżeli nie to ustaw Cmax na obecnej maszynie na koniec poprzedniego zadania na tej maszynie

                Cmax = std::max(prevCmax, curentMachineCmax) + N[j]->op[i].duration; // wybieraz wartość najwyższą i dodaj czas trwania obecnego zadania na obecnej maszynie 
            }
            N[j]->op[i].end = Cmax; // ustaw czas zakończenia obecnego zadania na obecnej maszynie równy Cmax
        }
    }
    return N[lastJob]->op[lastMachine].end; // ostatnie zadanie z ostatniej maszyny
}


/**
 * Szukanie najmniejszego czasu wykonywania.
 *
 * @param N Wektor operacji.
 * @param it Iterator najlepszej operacji.
 * @return Zwraca operację z najkrótszym czasem wykonywania.
 */
inline Job* minP(const std::vector<Job*> &N, int *it) {
    Job* minJob = N[0]; // wskaźnik na najlepsze zadanie
    *it = 0;

    for(int i = 0; i < N.size(); ++i) { // dla każdego zadania
        int currentFirst = N[i]->op[0].duration; // czas trwania obecnego zadania na pierwszej maszynie
        int currentLast = N[i]->op[minJob->last()].duration; // czas trwania obecnego zadania na ostatniej maszynie
        int bestFirst = minJob->op[0].duration; // czas trwania najlepszego zadania na pierwszej maszynie
        int bestLast = minJob->op[minJob->last()].duration; // czas trwania najlepszego zadania na ostatniej maszynie

        if(currentFirst < bestFirst) { // jeżeli czas na pierwszej maszynie jest mniejszy od najlepszego na pierwszej
            if (currentFirst < bestLast) { // jeżeli czas na pierwszej maszynie jest mniejszy od ostatniego na ostatniej
                minJob = N[i]; // ustaw tej jako najlepszy
                *it = i; // ustaw iterator na niego
            }
        }

        if(currentLast < bestFirst) { // jeżeli czas na ostatniej maszynie jest mniejszy od najlepszego na pierwszej
            if(currentLast < bestLast) { // jeżeli czas na ostatniej jest mniejszy od najlepszego na ostatniej
                minJob = N[i]; // ustaw tej jako najlepszy
                *it = i; // ustaw iterator na niego
            }
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
std::vector<Job*> Jonson(std::vector<Job*> N) {
    int l = 0; // indeks na początek listy zadań
    int k = N.size() - 1; // indeks na koniec listy zadań
    std::vector<Job*> Pi;
    Pi.resize(N.size());

    while(!N.empty()) { // dopuki są jakieś nieprzypisane zadania
        int it; 
        Job* minJob = minP(N, &it); // szukaj zadania z najkrótszym czasem trwania
        if(minJob->op[0].duration < minJob->op[minJob->op.size()-1].duration) { // jeżeli wyjkonuje się krócej na pierwszej maszynie
            Pi.at(l++) = minJob; // dopisz go od początku
        } else { // a jak nie
            Pi.at(k--) = minJob; // dopisz go od końca
        }
        N.erase(N.begin() + it); // usuń z nieuszeregowanych
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




int lowerBound(std::vector<Job*> N, std::vector<Job*> *Pi) {
    int lb = 0;
    Cemaks(*Pi);

    for (int i = 0; i < N[0]->op.size(); i++) {
        int sum = 0;
        for (int j = 0; j < N.size(); j++) {
            sum += N[j]->op[i].duration;
        }
        lb = std::max((*Pi)[Pi->size() - 1]->op[i].end + sum, lb);
    }
    return lb;
}



/**
 * Algorytm przeszukiwaina siłowego.
 *
 * @param N Wektor operacji.
 * @return Zwraca najlepszą kombinację.
 */
void BranchAndBound(int j, std::vector<Job*> N, std::vector<Job*> *result, int ub, std::vector<Job*> Pi = {}) {
    Pi.push_back(N[j]); // dodaj 
    N.erase(N.begin() + j);
    
    if(!N.empty()) {
        int lb = lowerBound(N, &Pi);
        if(lb < ub) {
            for (int i = 0; i < N.size(); i++) {
                BranchAndBound(i, N, result, ub, Pi);
            }
        }
    } else {
        int Cmax = Cemaks(Pi);
        if(Cmax < ub) {
            ub = Cmax;
            *result = Pi;
        }
    }
}


/**
 * Algorytm przeszukiwaina siłowego.
 *
 * @param N Wektor operacji.
 * @return Zwraca najlepszą kombinację.
 */
std::vector<Job*> InitBranchAndBound(std::vector<Job*> N) {
    int ub = Cemaks(Jonson(N)); // górne oszacowanie
    std::vector<Job*> Pi;

    for(int j = 0; j < N.size(); ++j) { // dla każdego zadania
        BranchAndBound(j, N, &Pi, ub); // wykonaj BnB
    }
    return Pi;
}