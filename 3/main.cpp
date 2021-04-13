#include <stdio.h>
#include <stdint.h>
#include <iostream> 

#include "scheduling.hpp"

std::vector<Job> generateOperations(const int &n, const int &m, const int &seed) {
    RandomNumberGenerator randf(seed);
    std::vector<Job> J;

    for(int j = 1; j <= n; ++j) J.emplace_back(Operation(j, 0, randf.nextInt(1, 29), 2), Operation(j, 0, randf.nextInt(1, 29), 1));
    
    return J;
}

int Cemaks(const std::vector<Job> &Pi) {
    int Cmax1 = 0;
    int Cmax2 = 0;
    printf("C: ");

    for(int j = 0; j < Pi.size(); ++j) {
        Cmax1 += Pi[j].op[0].duration;
        Cmax2 = std::max(Cmax1, Cmax2)  + Pi[j].op[1].duration;
        printf("[%d, %d] ", Cmax1, Cmax2);
    }
    printf("\n");
    return Cmax2;
}

void log(const char* name, const std::vector<Job> &pi) {
    printf("%s\npi: [", name);

    for(int i = 0; i < pi.size(); ++i) {
        printf("%3d", pi[i].op[0].number);
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
    log("Naturalna", J);
    log("Jonson", jonson(J));

    return 0;
}

