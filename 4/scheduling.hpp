#include "operation.hpp"
#include <algorithm>


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


