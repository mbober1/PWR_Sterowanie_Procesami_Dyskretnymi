#include <stdio.h>
#include <stdint.h>
#include "RandomNumberGenerator.h"
#include <iostream> 
#include <list>


int seed = 1;
int count = 10;


class job
{
public:
    int number, r, p, q; //numer, czas przygotowywania, czas wykonywania, czas stygniecia
    int s, c; //czas rozpoczecia, czas zakonczenia
    job(int number, int r, int p);
    job();
};

job::job(int number, int r, int p) : number(number), r(r), p(p) {}

job::job() {}


int max(int a, int b) {
    if(a>b) return a;
    else return b;
}

int min(int a, int b) {
    if(a<b) return a;
    else return b;
}

int calculate(job* jobs, int* pi, int count) {
    int Cmax = 0; //czas zakończenia 
    jobs[pi[0]].s = jobs[pi[0]].r; 
    jobs[pi[0]].c = jobs[pi[0]].s + jobs[pi[0]].p;

    for(uint8_t i = 1; i<count; ++i) {
        jobs[pi[i]].s = max(jobs[pi[i]].r, jobs[pi[i-1]].c);
        jobs[pi[i]].c = jobs[pi[i]].s + jobs[pi[i]].p;
        Cmax = max(Cmax, jobs[pi[i]].c + jobs[pi[i]].q);
    } 
    return Cmax;
}

int minR(std::list<job> N) {
    int t = (1<<16);
    for(std::list<job>::iterator it = N.begin(); it != N.end(); ++it) {
        t = min(t, it->r);
    }
    return t;
}


int* Schrage(job* jobs) {
    int k = 0;
    int* pi = new int[count];

    std::list<job> G; //zbiór zadań gotowych do realizacji
    std::list<job> N; //zbiór zadań nieuszeregowanych
    for(uint8_t i = 0; i<count; ++i)    N.push_back(jobs[i]);

    int t = minR(N);

    while (!G.empty() || !N.empty()) {
        while (!N.empty() && minR(N) <= t) {
            int temp = minR(N);
            for(std::list<job>::iterator it = N.begin(); it != N.end(); ++it) {
                if(it->r == temp) {
                    G.splice(G.begin(), N, it);
                    break;
                }
            }
        }

        if(!G.empty()) {
            printf("%d, ",G.back().number + 1);
            pi[k++] = G.back().number;
            t =+ G.back().p;
            G.pop_back();
        } else {
            t = minR(N);
        }
        
    }
    return pi;
}


int main() {
    // std::cin >> count;
    // std::cin >> seed;
    RandomNumberGenerator randf(seed);
    int A = 0;
    int X = 29; //bo tak


    job* jobs = new job[count];
    int* pi = new int[count];

    //generowanie instancji
    for(uint8_t i = 0; i<count; ++i) { 
        jobs[i].number = i; //nadawanie numerów
        pi[i] = i;
    }

    for(uint8_t i = 0; i<count; ++i) {
        jobs[i].p = randf.nextInt(1, 29); //losowanie czasu wykonywania
        A += jobs[pi[i]].p; 
    }

    for(uint8_t i = 0; i<count; ++i) {
        jobs[i].r = randf.nextInt(1, A); //losowanie czasu przygotowywania
    }

    for(uint8_t i = 0; i<count; ++i) {
        jobs[i].q = randf.nextInt(1, A); //losowanie czasu przygotowywania
    }

    
    int Cmax = calculate(jobs, pi, count);
    
    // delete pi;
    pi = Schrage(jobs);

    //wypisywanie wyników
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

    printf("]\nq:  [");

    for(uint8_t i = 0; i<count; ++i) {
        printf("%3d, ", jobs[pi[i]].q);
    }

    printf("]\nPermutacja naturalna\n\npi: [");

    for(uint8_t i = 0; i<count; ++i) {
        printf("%3d, ", pi[i] + 1);
    }

    printf("]\nS:  [");

    for(uint8_t i = 0; i<count; ++i) {
        printf("%3d, ", jobs[pi[i]].s);
    }

    printf("]\nC:  [");

    for(uint8_t i = 0; i<count; ++i) {
        printf("%3d, ", jobs[pi[i]].c);
    }

    printf("]\nCq:  [");

    for(uint8_t i = 0; i<count; ++i) {
        printf("%3d, ", jobs[pi[i]].c + jobs[pi[i]].q);
    }
    printf("]\nCmax: %d\n", Cmax);

    return 0;
}

