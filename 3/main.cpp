#include <stdio.h>
#include <stdint.h>
#include <iostream> 

#include "scheduling.hpp"

std::vector<Job> generateOperations(int n, int m, int seed) {
    RandomNumberGenerator randf(seed);
    std::vector<Job> J;

    for(int j = 1; j <= n; ++j) J.emplace_back(Operation(j, 0, randf.nextInt(1, 29), 2), Operation(j, 0, randf.nextInt(1, 29), 1));
    
    return J;
}

int Cemaks(std::vector<Job> Pi) {
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


int main() {

    std::vector<Job> J = generateOperations(6, 2, 1);
    
    printf("Cmax %d \n", Cemaks(J));

    return 0;
}

