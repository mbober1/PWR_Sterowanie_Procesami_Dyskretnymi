
#include "RandomNumberGenerator.h"
#include "scheduling.hpp"

std::vector<Job> generateOperations(const int &n, const int &m, const int &seed) {
    RandomNumberGenerator randf(seed);
    std::vector<Job> J;

    for(int j = 1; j <= n; ++j) {
        // Job* tmp = new Job(Operation(j, 0, randf.nextInt(1, 29), 2), Operation(j, 0, randf.nextInt(1, 29), 1));
        J.emplace_back(Operation(j, 0, randf.nextInt(1, 29), 2), Operation(j, 0, randf.nextInt(1, 29), 1));
        // J.emplace_back(tmp)
    }
    
    return J;
}

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
    int seed = 1;
    int n = 6;
    int m = 2;

    printf("seed: %d\nrozmiar: %dx%d\n\n", seed, n, m);

    std::vector<Job> J = generateOperations(n, m, seed);

    std::vector<Job*> J2;

    for(int i = 0; i < J.size(); ++i) {
        J2.push_back(&J[i]);
    }

    log("Naturalna", J2);
    // log("Jonson", Jonson(J));
    log("BF", BruteForce(J2));

    return 0;
}

