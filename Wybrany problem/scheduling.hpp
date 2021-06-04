#include <vector>
#include "operation.hpp"
#include "../RandomNumberGenerator.h"
#include <stdio.h>
#include <queue>
#include <functional>
#include <algorithm>
#include <tuple>
#include <map>
#include "graf.hpp"
#include <bits/stdc++.h>


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
            int Cmax; // czas zakończenia aktualnego zadania
            int prevCmax; // poprzednie Cmax
            int curentMachineCmax; // obecne Cmax

            int currentOpDuration = N[j]->op[i].duration; // czas trwania obecnego zadania

            if(i == 0) { // jeżeli to jest pierwsza maszyna
                prevCmax = 0;

                if(j == 0) Cmax = 0 + currentOpDuration; // jeżeli to jest pierwsze zadanie to Cmax ustawiamy na jego czas trwania
                else Cmax = N[j-1]->op[i].end + currentOpDuration; // jeśli nie to Cmax to czas jego trwania plus koniec trwania poprzedniego

            } else { // jeżeli to nie jest pierwsza maszyna
                prevCmax = N[j]->op[i-1].end; // jeśli nie to prevCmax ustaw na koniec tego zadania w poprzedniej maszynie

                if(j == 0) curentMachineCmax = 0; // jeżeli to jest pierwsze zadanie to ustaw Cmax na obecnej maszynie na 0
                else curentMachineCmax = N[j-1]->op[i].end; // jeżeli nie to ustaw Cmax na obecnej maszynie na koniec poprzedniego zadania na tej maszynie

                Cmax = std::max(prevCmax, curentMachineCmax) + currentOpDuration; // wybieraz wartość najwyższą i dodaj czas trwania obecnego zadania na obecnej maszynie 
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

    while(!N.empty()) { // dopóki są jakieś nieprzypisane zadania
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
 * Liczenie ścieżki krytycznej
 *
 * @param Pi Wektor zadań.
 * @return Zwraca wektor z informacjami o ścieżce krytycznej
 */
std::vector<std::tuple<int, int>> CriticalPath(std::vector<Job*> Pi) {
    int m = Pi[0]->op.size() - 1; // aktualna maszyna
    auto n = Pi.back(); // aktualne zadanie
    int idx = Pi.size() - 1;  // index aktualnego joba w wektorze Pi
    std::vector<std::tuple<int, int>> J; // wektor z elementami ścieżki krytycznej (indeks zadania, indeks maszyny)
    Cemaks(Pi); // ustaw poprawnie tablice S i C

    J.push_back({idx, m}); // dorzuć ostatnie zadanie do ścieżki

    while(idx > 0) { // dopóki nie jesteśmy na pierwszym zadaniu
        int start = n->op[m].end - n->op[m].duration; // czas rozpoczęcia zadania 
        int prevEnd = Pi[idx-1]->op[m].end; // czas końca poprzedniego zadania

        if(prevEnd == start) { // jeśli nowe zadanie zaczyna się tam gdzie poprzednie kończy
            idx--; // zmień zadanie na poprzednie
            J.push_back({idx, m}); // dorzuć kolejne zadanie do ścieżki
            n = Pi[idx];
        } else { // jeśli nie
            m--; // zmień maszynę na poprzednią
            J.push_back({idx, m}); // dorzuć kolejne zadanie do ścieżki
        }
    }

    return J;
}


/**
 * Wybierz zadanie z dłuższą sumą wykonywania operacji.
 *
 * @param a Zadanie 1.
 * @param b Zadanie 2
 * @return Zwraca prawdę czy A < B
 */
bool CompareSumP(Job* a, Job* b) {
    int aSum = 0;
    int bSum = 0;

    for (size_t i = 0; i < a->op.size(); i++)
    {
        aSum += a->op[i].duration; // policz czasy zadania A
    }

    for (size_t i = 0; i < b->op.size(); i++)
    {
        bSum += b->op[i].duration; // policz czasy zadania B
    }

    return aSum < bSum;
}


/**
 * Znajdź najlepsze miejsce dla zadania.
 *
 * @param Pi Wektor operacji.
 * @param job Zadanie do wstawienia.
 * @return Zwraca wektor z dodanym zadaniem.
 */
std::vector<Job*> NehInsert(std::vector<Job*> Pi, Job* job) {
    int minCmax = 1<<16; // minimalny Cmax
    int idx;

    for (size_t l = 0; l <= Pi.size(); l++) // dla każdej możliwej pozycji
    {
        Pi.insert(Pi.begin() + l, job); // dodaj tymczasowo zadanie na danej pozycji
        int tmp = Cemaks(Pi); // policz Cmax

        if(tmp < minCmax) { // jeżeli okaże się mniejszy
            minCmax = tmp;
            idx = l;
        }

        Pi.erase(Pi.begin() + l); // usuń zadanie z tej pozycji 
    }

    Pi.insert(Pi.begin() + idx, job); // dodaj zadanie w najlepszej znalezionej pozycji
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


/**
 * Algorytm NEH.
 *
 * @param N Wektor operacji.
 * @return Zwraca najlepszą kombinację.
 */
std::vector<Job*> NEH(std::vector<Job*> N, int upgrade = 0) {
    std::vector<Job*> Pi;
    
    std::sort(N.begin(), N.end(), CompareSumP); // posortuj od najdłuższych czasów wykonania

    while(!N.empty()) { // gdy są jakieś zadania
        auto j = N.back(); // weź to z najkrótszym czasem
        Pi = NehInsert(Pi, j); // wstaw w najlepszej pozycji
        N.pop_back(); // usuń z nieuszeregowanych


        if(upgrade) { // jeżeli wybierzemy upgrade
            auto path = CriticalPath(Pi); // policz ścieżkę krytyczną
            Job* extraJob; // znalezione zadanie
            int extraIdx; // indeks znalezionego zadania

            switch (upgrade)
            {
            case 1: { // Zadanie zawierające najdłuższą operacje na ścieżce krytycznej
                    int max = 0; // maksymalny czas trwania operacji

                    for (size_t i = 0; i < path.size(); i++) // dla każdej operacji na ścieżce
                    {
                        auto job = Pi[std::get<0>(path[i])];
                        auto duration = job->op[std::get<1>(path[i])].duration;

                        if(duration > max) {
                            max = duration;
                            extraJob = job;
                            extraIdx = std::get<0>(path[i]);
                        }
                    }                
                    break;
                }

            case 2: { // Zadanie zawierające największą sumę operacji wchodzących w ścieżkę krytyczną
                    std::map<int, int> durations; // mapa czasów trwania
                    int max = 0; // największa ilość wystąpień zadania

                    for (size_t i = 0; i < path.size(); i++) // dla każdej operacji na ścieżce
                    {
                        auto idx = std::get<0>(path[i]);
                        auto job = Pi[idx];
                        auto duration = job->op[std::get<1>(path[i])].duration;
                        
                        durations[idx] += duration;
                    }

                    
                    for (auto const& x : durations) { // dla każdego elementu w mapie
                        if(x.second > max)  { // jeżeli czas trwania operacji jest większy
                            extraIdx = x.first;
                            max = x.second;
                        }
                    }

                    extraJob = Pi[extraIdx];
                    
                    break;
                }

            }

            Pi.erase(Pi.begin() + extraIdx); // usuń wybrane zadanie
            Pi = NehInsert(Pi, extraJob); // znajdź mu lepszą pozycję
        }

    }
    return Pi;
}



/**
 * Zamiana dwóch elementów w wektorze
 *
 * @param N Wektor operacji.
 * @param A Element A.
 * @param B Element B.
 * @return Zwraca najlepszą kombinację.
 */
std::vector<Job*> swap(std::vector<Job*> N, int A, int B) {
    auto tmp = N[A];
    N[A] = N[B];
    N[B] = tmp;
    return N;
}


/**
 * Algorytm Symulowane wyżarzanie.
 *
 * @param N Wektor operacji.
 * @param T Temperatura startowa.
 * @param Tend Temperatura końcowa.
 * @param L Ilość epok.
 * @return Zwraca najlepszą kombinację.
 */
std::vector<Job*> SimulatedAnnealing(std::vector<Job*> N, int T, int Tend, int L) {
    std::vector<Job*> Pi = N;
    auto PiBest = Pi; // najlepsza kombinacja

    while(T > Tend) { // dopóki jest temperatura
        for (size_t k = 0; k < L; k++) // dla każdej epoki
        {
            int i = rand() % Pi.size(); // losuj który element zamienić
            int j;
            do // losuj drugi element do zamiany inny niż 'i'
            {
                j = rand() % Pi.size();
            } while (i == j);
            
            auto PiNew = swap(Pi, i, j); // zamień elementy
            auto CmaxDiff = Cemaks(Pi) - Cemaks(PiNew); // policz różnicę Cmax po zmianie

            if(CmaxDiff <= 0) { // jeżeli jest gorzej
                double r = ((double)rand())/RAND_MAX; // wylosuj prawdopodobieństwo przyjęcia
                if (r >= exp(CmaxDiff/T)) PiNew = Pi; // jeżeli jest duże to przyjmij gorszą kombinację
            }

            Pi = PiNew; // zapisz nową kombinację
            if(Cemaks(Pi) < Cemaks(PiBest)) PiBest = Pi; // jeżeli Cmax okaże się najlepszy to też go zapisz
        }

        T -= 1; // zmniejsz temperaturę
    }
    return Pi;
}


/**
 * Algorytm Tabu.
 *
 * @param N Wektor operacji.
 * @param itLimit ilość iteracji.
 * @param cadence ilość kadencji (zablokowania na liście tabo).
 * @return Zwraca najlepszą kombinację.
 */
std::vector<Job*> TabuSearch(std::vector<Job*> N, int itLimit, int cadence) {
    std::vector<Job*> Pi = NEH(N); // zacznij od NEHa
    auto PiBest = Pi; // najlepsza kombinacja
    int** tabu = new int*[Pi.size()]; // tablica tabo

    for (size_t i = 0; i < Pi.size(); i++) // dla każdej kolumny
    {
        tabu[i] = new int[Pi.size()]; // zaalokuj dynamicznie pamięć
    }
    

    for (size_t it = 0; it < itLimit; it++) // dla każdej iteracji
    {
        int jBest = 0; // najlepsze j
        int kBest = 1; // najlepsze k
        int CBest = INT_MAX; // najlepsze C

        for (size_t j = 0; j < Pi.size()-1; j++) // dla każdego elementu
        {
            for (size_t k = j+1; k < Pi.size()-1; k++) // dla każdego elementu
            {
                if(tabu[j][k] < it) { // jeżeli nie znajduje się na liście tabo
                    auto PiNew = swap(Pi, j ,k); // zamień elementy
                    if(Cemaks(PiNew) < CBest) { // jeżeli Cmax okaże się lepszy
                        CBest = Cemaks(PiNew); // zapisz Cmax
                        jBest = j; // zapisz element j
                        kBest = k; // zapisz element k
                    } 
                }
            }
        }
        Pi = swap(Pi, jBest, kBest); // zamień tak aby uzyskać najlepszą kombinację
        tabu[jBest][kBest] = it + cadence; // uaktualnij listę tabo
        if(Cemaks(Pi) < Cemaks(PiBest)) PiBest = Pi; // zapisz najlepszą kombinację
    }
    
    return PiBest;
}
