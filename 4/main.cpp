
#include "RandomNumberGenerator.h"
#include "scheduling.hpp"


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
        Job* tmp = new Job();
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
        J[j]->deadline = randf.nextInt(1, A);
    }

    return J;
}



/**
 * Wypisywanie raportu.
 *
 * @param name Nazwa zadania.
 * @param pi Wektor z uporządkowanymi zadaniami.
 */
// void log(const char* name, const std::vector<Job*> &pi) {
//     printf("%s\npi: [", name);

//     for(int i = 0; i < pi.size(); ++i) {
//         printf("%3d", pi[i]->op[0].number);
//         if(i != pi.size() - 1) printf(", ");
//     }
//     printf("]\n");
//     printf("Cmax %d \n", Cemaks(pi));
//     printf("\n\n");
// }


int main() {
    int seed = 1;
    int n = 3;

    printf("seed: %d\nrozmiar: %d\n\n", seed, n);

    std::vector<Job*> J = generateOperations(n, seed);

    // log("Naturalna", J);
    // log("Jonson", Jonson(J));
    // log("BruteForce", BruteForce(J));
    // log("BnB", initBranchAndBound(J));

    return 0;
}

