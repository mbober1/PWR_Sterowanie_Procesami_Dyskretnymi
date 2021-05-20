#include <stdio.h>
#include <stdint.h>
#include "../RandomNumberGenerator.h"
#include <iostream> 


class job
{
public:
    uint8_t number, r, p; //numer, czas przygotowywania, czas wykonywania
    uint16_t s, c; //czas rozpoczecia, czas zakonczenia
    job(uint8_t number, uint8_t r, uint8_t p);
    job();
};

job::job(uint8_t number, uint8_t r, uint8_t p) : number(number), r(r), p(p) {}

job::job() {}


int max(int a, int b) {
    if(a>b) return a;
    else return b;
}

int main() {
    int seed, count;
    std::cin >> count;
    std::cin >> seed;
    RandomNumberGenerator randf(seed);
    uint16_t A = 0;
    uint16_t C = 0;


    job* jobs = new job[count];
    uint8_t* pi = new uint8_t[count];

    for(uint8_t i = 0; i<count; ++i) {
        jobs[i].number = i;
        pi[i] = i;
    }

    for(uint8_t i = 0; i<count; ++i) {
        jobs[i].p = randf.nextInt(1, 29);
        A += jobs[pi[i]].p;
    }

    for(uint8_t i = 0; i<count; ++i) {
        jobs[i].r = randf.nextInt(1, A);
    }



    jobs[pi[0]].s = jobs[pi[0]].r;
    jobs[pi[0]].c = jobs[pi[0]].s + jobs[pi[0]].p;

    for(uint8_t i = 1; i<count; ++i) {
        jobs[pi[i]].s = max(jobs[pi[i]].r, jobs[pi[i-1]].c);
        jobs[pi[i]].c = jobs[pi[i]].s + jobs[pi[i]].p;
    } 


    printf("Źródło losowania: %d \nRozmiar problemu: %d\n", seed, count);
    printf("nr: [");

    for(uint8_t i = 0; i<count; ++i) {
        printf("%3d, ", jobs[pi[i]].number + 1);
    }

    printf("]\nr:  [");

    for(uint8_t i = 0; i<count; ++i) {
        printf("%3d, ", jobs[pi[i]].r);
    }

    printf("]\np:  [");

    for(uint8_t i = 0; i<count; ++i) {
        printf("%3d, ", jobs[pi[i]].p);
    }

    printf("]\n\npi: [");

    for(uint8_t i = 0; i<count; ++i) {
        printf("%3d, ", pi[i] + 1);
    }

    printf("]\nC:  [");

    for(uint8_t i = 0; i<count; ++i) {
        printf("%3d, ", jobs[pi[i]].c);
    }
    printf("]\n");

    return 0;
}

