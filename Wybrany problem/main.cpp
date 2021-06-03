
#include "scheduling.hpp"
#include <chrono>
#include <iostream>
#include <math.h>


/**
 * Generuje dane.
 *
 * @param n Ilość zadań.
 * @param m Ilość maszyn.
 * @return Zwraca nieuporządkowane zadania.
 */
std::vector<Job*> generateOperations(const int &n, const int &m, const int &seed) {
    RandomNumberGenerator randf(seed);
    std::vector<Job*> J;

    for(int j = 1; j <= n; ++j) {
        Job* tmp = new Job();
        for(int i = 1; i <= m; ++i) {
            tmp->addOperation(Operation(j, randf.nextInt(1, 29)));
        }
        J.push_back(tmp);
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
    printf("%s\npi: [", name);

    for(int i = 0; i < pi.size(); ++i) {
        printf("%3d", pi[i]->op[0].number);
        if(i != pi.size() - 1) printf(", ");
    }
    printf("]\n");
    printf("Cmax %d \n", Cemaks(pi));
    printf("\n\n");
}


int main() {
    int seed = 609;
    int n = 8;
    int m = 5;

    printf("seed: %d\nrozmiar: %dx%d\n\n", seed, n, m);
    std::vector<Job*> J = generateOperations(n, m, seed);

    log("Naturalna", J);
    log("Jonson", Jonson(J));
    log("NEH", NEH(J));
    // log("NEH", NEH(J, 1));
    // log("NEH", NEH(J, 2));
    log("BF", BruteForce(J));
    log("Wyzarzanie", SimulatedAnnealing(J, 200, 0, 200));
    log("Tabu", TabuSearch(J, 200000, 5));


    return 0;
}

