#include "operation.hpp"


int minDead(const std::vector<Job*> &N) {
    Job* J; // wskaźnik na najlepszy
    J = N[0]; // przypisujemy pierwszy
    int idx = 0;

    for (size_t i = 1; i < N.size(); i++)
    {
        if(N[i]->deadline < J->deadline) {
            J = N[i];
            idx = i;
        }
    }
    return idx;
}


/**
 * Algorytm Greedy.
 *
 * @param N Wektor operacji.
 * @return Zwraca najlepszą kombinację.
 */
std::vector<Job*> Greedy(std::vector<Job*> N) {
    std::vector<Job*> Pi;

    while(!N.empty()) {
        int best = minDead(N);
        Pi.push_back(N[best]);
        N.erase(N.begin() + best);
    }

    return Pi;
}