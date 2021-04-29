
#include "RandomNumberGenerator.h"
#include "scheduling.hpp"
#include <stdio.h>


/**
 * Generuje dane.
 *
 * @param n Ilość zadań.
 * @param seed Ziarno losowania.
 * @return Zwraca nieuporządkowane zadania.
 */
std::vector<Job*> generateOperations(const int &n, const int &seed) {
    RandomNumberGenerator randf(seed);
    std::vector<Job*> J;
    int A = 0;

    for(int j = 1; j <= n; ++j) {
        Job* tmp = new Job(j);
        J.push_back(tmp);
    }

    for(int j = 0; j < n; ++j) {
        J[j]->processingTime = randf.nextInt(1, 29);
        A += J[j]->processingTime;
    }

    for(int j = 0; j < n; ++j) {
        J[j]->weight = randf.nextInt(1, 9);
    }    

    for(int j = 0; j < n; ++j) {
        J[j]->deadline = randf.nextInt(1, 29);
    }

    return J;
}



/**
 * Wypisywanie raportu.
 *
 * @param name Nazwa zadania.
 * @param pi Wektor z uporządkowanymi zadaniami.
 */
void log(const char* name, const std::vector<Job*> &pi) {
    int size = pi.size();
    int *C = new int[size];
    int *T = new int[size];

    printf("%s\npi: [", name);
    for(int i = 0; i < size; ++i) {
        printf("%3d", pi[i]->number);

        if(i != size - 1) printf(", ");
    }
    printf("]\n");


    printf("C:  [");
    for(int i = 0; i < size; ++i) {
        if(i) C[i] = C[i-1] + pi[i]->processingTime;
        else C[i] = pi[i]->processingTime;
        
        printf("%3d", C[i]);
        if(i != size - 1) printf(", ");
    }
    printf("]\n");


    printf("T:  [");
    for(int i = 0; i < size; ++i) {
        T[i] = std::max(C[i] - pi[i]->deadline, 0);
        printf("%3d", T[i]);
        if(i != size - 1) printf(", ");
    }
    printf("]\n");

    printf("wT: [");
    int wiTi = 0;
    for(int i = 0; i < size; ++i) {
        int wT = T[i] * pi[i]->weight;
        wiTi += wT;
        printf("%3d", wT);
        if(i != size - 1) printf(", ");
    }
    printf("]\n");


    printf("wiTi: %d\n\n", wiTi);

    // delete[] C;
    // delete[] T;
}


int main() {
    int seed = 1;
    int n = 8;

    printf("seed: %d\nrozmiar: %d\n\n", seed, n);

    std::vector<Job*> J = generateOperations(n, seed);

    log("Naturalna", J);
    log("Greedy", Greedy(J));
    log("BruteForce", BruteForce(J));
    // log("Dynamic", Dynamic(J));
    Dynamic(J);

    return 0;
}

