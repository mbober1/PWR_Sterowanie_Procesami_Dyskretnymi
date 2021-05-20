#include <stdio.h>
#include <stdint.h>
#include "../RandomNumberGenerator.h"
#include <iostream> 

#include "scheduling.hpp"


void log(const char* name, std::vector<job> &jobs, const std::vector<Pi> &pi) {
    printf("%s\npi: [", name);

    for(uint8_t i = 0; i < pi.size(); ++i) {
        printf("%3d", pi[i].number + 1);
        if(i != pi.size() - 1) printf(", ");
    }

    printf("]\nS:  [");

    for(uint8_t i = 0; i < pi.size(); ++i) {
        printf("%3d", pi[i].s);
        if(i != pi.size() - 1) printf(", ");
    }

    printf("]\nC:  [");

    for(uint8_t i = 0; i < pi.size(); ++i) {
        printf("%3d", pi[i].c);
        if(i != pi.size() - 1) printf(", ");
    }

    printf("]\nCq: [");

    for(uint8_t i = 0; i < pi.size(); ++i) {
        printf("%3d", pi[i].c + jobs[pi[i].number].q);
        if(i != pi.size() - 1) printf(", ");
    }

    printf("]\nCmax: %d\n\n", cmax(jobs, pi));
}

std::vector<job> generateJobs(int size, int seed) {
    RandomNumberGenerator randf(seed);
    int A = 0;
    int X = 29; //bo tak
    std::vector<job> jobs; //zadania

    // //generowanie instancji
    for(uint8_t i = 0; i<size; ++i) { 
        jobs.emplace_back(i);
    }

    for(uint8_t i = 0; i<size; ++i) {
        jobs[i].p = randf.nextInt(1, 29); //losowanie czasu wykonywania
        A += jobs[i].p; 
    }

    for(uint8_t i = 0; i<size; ++i) {
        jobs[i].r = randf.nextInt(1, A); //losowanie czasu przygotowywania
    }

    for(uint8_t i = 0; i<size; ++i) {
        jobs[i].q = randf.nextInt(1, A); //losowanie czasu stygniecia
    }

    return jobs;
}

void printJobsInfo(std::vector<job> jobs, int size, int seed) {
    printf("Źródło losowania: %d \nRozmiar problemu: %d\n\n", seed, size);

    printf("nr: [");

    for(uint8_t i = 0; i < jobs.size(); ++i) {
        printf("%3d", jobs[i].number + 1);
        if(i != jobs.size() - 1) printf(", ");
    }

    printf("]\nr:  [");

    for(uint8_t i = 0; i < jobs.size(); ++i) {
        printf("%3d", jobs[i].r);
        if(i != jobs.size() - 1) printf(", ");
    }

    printf("]\np:  [");

    for(uint8_t i = 0; i < jobs.size(); ++i) {
        printf("%3d", jobs[i].p);
        if(i != jobs.size() - 1) printf(", ");
    }

    printf("]\nq:  [");

    for(uint8_t i = 0; i < jobs.size(); ++i) {
        printf("%3d", jobs[i].q);
        if(i != jobs.size() - 1) printf(", ");
    }
    printf("\n\n");
}

int main() {
    int size = 10;
    int seed = 7523;
    std::cin >> size;
    std::cin >> seed;


    std::vector<job> jobs = generateJobs(size, seed);
    printJobsInfo(jobs, size, seed);

    std::vector<Pi> pi;
    log("[Wektor] Kolejność po alg Schrage", jobs, Schrage(jobs));
    log("[Kopiec] Kolejność po alg Schrage", jobs, SchrageHeap(jobs));
    log("[Wektor] Kolejność po alg Schrage z przerwaniami", jobs, SchragePmtn(jobs));
    log("Carlier", jobs, Carlier(jobs));


    return 0;
}

