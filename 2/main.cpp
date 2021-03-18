#include <stdio.h>
#include <stdint.h>
#include "RandomNumberGenerator.h"
#include <iostream> 
#include <vector>
#include <algorithm>


int seed = 7523;
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

int minR(std::vector<job> N) {
    int t = (1<<16);
    for(std::vector<job>::iterator it = N.begin(); it != N.end(); ++it) {
        t = min(t, it->r);
    }
    return t;
}


int minRit(std::vector<job> N) {
    int t = minR(N);
    for(int i = 0; i < N.size(); ++i) if(N[i].r == t) return i;
    return -1;
}


int maxQ(std::vector<job> N) {
    int t = 0;
    for(std::vector<job>::iterator it = N.begin(); it != N.end(); ++it) {
        if(it->q > N[t].q) t = it-N.begin();
    }
    return t;
}

bool porównaj(const job& A, const job& B) {
    if (A.r < B.r) return true;
    if (A.r > B.r) return false;

    if (A.q > B.q) return true;
    if (A.q < B.q) return false;

    return false; 
}

struct {
    bool operator() (const job& A, const job& B) const {
        if (A.r < B.r) return true;
        if (A.r > B.r) return false;

        if (A.q > B.q) return true;
        if (A.q < B.q) return false;

        return false; 
    }
} abcd;



int* SchragePmtn(job* jobs) {
    int k = 0;
    int* pi = new int[100];

    std::vector<job> G; //zbiór zadań gotowych do realizacji
    std::vector<job> N; //zbiór zadań nieuszeregowanych
    for(uint8_t i = 0; i<count; ++i)    N.push_back(jobs[i]);

    // std::sort(N.begin(), N.end(), abcd);
    int t = minR(N);

    while (!G.empty() || !N.empty()) {
        int Cmax = calculate(jobs, pi, k);

        while (!N.empty() && minR(N) <= t) { //faza 1
            // std::sort(N.begin(), N.end(), abcd);
            int temp = minRit(N);
            G.push_back(N[temp]);
            N.erase(N.begin() + temp);
        }

        if(!G.empty()) { //faza 2
            int j = maxQ(G);
            // int temp = minRit(N);
            // if(G[j].q < N[temp].q) { //faza 3
            //     printf("powinno być %d, zamiast %d\n", N[temp].number + 1, G[j].number);
            //     t += N[temp].q; //3
            //     G[j].p += t - N[temp].r; //4
            //     pi[k++] = N[temp].number;
            //     N.erase(N.begin() + temp);
            //     N.push_back(G[j]);
            //     G.erase(G.begin() + j); 
            // } else {
                pi[k++] = G[j].number;
                t += G[j].p;

                std::sort(N.begin(), N.end(), abcd);
                int dupa1 = minRit(N);
                if (N[dupa1].r <= t && N[dupa1].q > G[j].q) { 
                    printf("Zadanie %d przerywa zadanie %d!\n", N[dupa1].number+1, G[j].number+1);
                    int aaa = t - N[dupa1].r;
                    G[j].p = aaa - G[j].r;
                    N.push_back(G[j]);
                    N.erase(N.begin() + dupa1);
                }

                G.erase(G.begin() + j);
                
            // }

        } else {
            t = minR(N);
        }
    }
    return pi;
}


void printuj(const char* name, job* jobs, int count, int Cmax, int pi[]) {
//wypisywanie wyników
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

    printf("]\n%s\n\npi: [", name);

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

    printf("]\nCq: [");

    for(uint8_t i = 0; i<count; ++i) {
        printf("%3d, ", jobs[pi[i]].c + jobs[pi[i]].q);
    }
    printf("]\nCmax: %d\n", Cmax);
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

    printf("Źródło losowania: %d \nRozmiar problemu: %d\n", seed, count);
    

    int Cmax = calculate(jobs, pi, count);
    printuj("Permutacja Naturalna", jobs, count, Cmax, pi);

    pi = SchragePmtn(jobs);
    Cmax = calculate(jobs, pi, count);
    printuj("Kolejność po alg Schrage", jobs, count, Cmax, pi);



    

    return 0;
}

