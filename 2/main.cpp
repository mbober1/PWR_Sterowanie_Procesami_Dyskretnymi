#include <stdio.h>
#include <stdint.h>
#include "RandomNumberGenerator.h"
#include <iostream> 
#include <vector>
#include <algorithm>


int seed = 1;
int count = 6;

class Pi {
public:
    int s; // początek wykonywania zadania
    int c; // koniec wykonywania zadania
    int number; // numer zadania
    Pi(int s, int c, int number);
};

Pi::Pi(int s, int c, int number): s(s), c(c), number(number) {}

class job
{
public:
    int number, r, p, q; //numer, czas przygotowywania, czas wykonywania, czas stygniecia
    job(int number, int r, int p);
    job(int number);
};

job::job(int number, int r, int p) : number(number), r(r), p(p) {}

job::job(int number) : number(number) {}


inline int max(int a, int b) {
    if(a>b) return a;
    else return b;
}

inline int min(int a, int b) {
    if(a<b) return a;
    else return b;
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

struct {
    bool operator() (const job& A, const job& B) const {
        if (A.r < B.r) return true;
        if (A.r > B.r) return false;

        if (A.q > B.q) return true;
        if (A.q < B.q) return false;

        return false; 
    }
} abcd;

std::vector<Pi> SchragePmtn(const std::vector<job> &jobs) {
    std::vector<Pi> pi; //lista wykonywania
    std::vector<job> G; //zbiór zadań gotowych do realizacji
    std::vector<job> N; //zbiór zadań nieuszeregowanych

    // Inicjalizacja listy zadań nieuszeregowanych
    for(uint8_t i = 0; i<jobs.size(); ++i) N.push_back(jobs[i]);
    std::sort(N.begin(), N.end(), abcd);

    // Ustawiamy aktualny czas na początek najwcześniejszego zadania
    int t = N[0].r;

    // Działamy dopóki zostały zadania
    while (!G.empty() || !N.empty()) {
        // Jeżeli jest nieuszeregowane zadanie które jest już gotowe...
        while (!N.empty() && N[0].r <= t) {
            // to przerzucamy je do G
            G.push_back(N[0]);
            N.erase(N.begin());
            std::sort(N.begin(), N.end(), abcd);
        }

        // Jeśli nie ma gotowych zadań to przesuwamy czas do najbliższego zadania
        if (G.empty()) t = N[0].r;
        // w przeciwnym wypadku...
        else {
            // szukamy zadania najdluzej stygnącego
            int j = maxQ(G);
            // zwiększamy czas... 
            printf("Zaczynamy zadanie %d\n", G[j].number+1);
            pi.emplace_back(t, t+G[j].p, G[j].number);
            // printf("[%d] S: %d, C: %d\n", pi[pi.size() - 1].number, pi[pi.size() - 1].s, pi[pi.size() - 1].c);
            t += G[j].p;

            // Jeżeli jest nieuszeregowane zadanie które dlużej stygnie
            if (!N.empty() && N[0].r <= t && N[0].q > G[j].q) {
                printf("Zadanie %d przerywa zadanie %d!\n", N[0].number+1, G[j].number+1);

                // Ustawiamy aktualnemu zadaniu nowy czas trwania (pomniejszony o to co już wykonał)
                G[j].p = t - N[0].r;

                // Cofamy się w czasie
                t = N[0].r;

                // Aktualizacja czasu skonczenia zadania
                // printf("Zmiana [%d] C: %d -> %d\n", pi[pi.size() - 1].number, pi[pi.size() - 1].c, t);
                pi[pi.size() - 1].c = t;

                // Przenosimy aktualne zadanie do nieuszeregowanych i usuwamy to zadanie
                N.push_back(G[j]);
                G.erase(G.begin() + j);

                // Dodajemy nowe zadanie do gotowych do wykonania i usuwamy je z nieuszeregowanych
                G.push_back(N[0]);
                N.erase(N.begin() + 0);

                std::sort(N.begin(), N.end(), abcd);
                j = maxQ(G);
            }
            // i usuwamy to zadanie
            else G.erase(G.begin() + j);
        }
    }

    return pi;
}

int cmax(std::vector<job> &jobs, const std::vector<Pi> &pi) {
    int c = 0;
    for (auto &job : pi) {
        int tmp = job.c + jobs[job.number].q;
        if (tmp > c) c = tmp;
    }

    return c;
}

void log(const char* name, std::vector<job> &jobs, const std::vector<Pi> &pi) {
    printf("]\n\n%s\npi: [", name);

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

    printf("]\nCmax: %d\n", cmax(jobs, pi));
}

int main() {
    // std::cin >> count;
    // std::cin >> seed;
    RandomNumberGenerator randf(seed);
    int A = 0;
    int X = 29; //bo tak


    std::vector<job> jobs; //zadania
    std::vector<Pi> pi;   //kolejność zadań

    // //generowanie instancji
    for(uint8_t i = 0; i<count; ++i) { 
        jobs.emplace_back(i);
        // pi.push_back(i);
    }

    for(uint8_t i = 0; i<count; ++i) {
        jobs[i].p = randf.nextInt(1, 29); //losowanie czasu wykonywania
        A += jobs[i].p; 
    }

    for(uint8_t i = 0; i<count; ++i) {
        jobs[i].r = randf.nextInt(1, A); //losowanie czasu przygotowywania
    }

    for(uint8_t i = 0; i<count; ++i) {
        jobs[i].q = randf.nextInt(1, A); //losowanie czasu przygotowywania
    }

    // int Cmax = calculate(jobs, pi);


    // printf("Źródło losowania: %d \nRozmiar problemu: %d\n\n", seed, count);

    // printf("nr: [");

    // for(uint8_t i = 0; i < jobs.size(); ++i) {
    //     printf("%3d", jobs[pi[i]].number + 1);
    //     if(i != jobs.size() - 1) printf(", ");
    // }

    // printf("]\nr:  [");

    // for(uint8_t i = 0; i < jobs.size(); ++i) {
    //     printf("%3d", jobs[pi[i]].r);
    //     if(i != jobs.size() - 1) printf(", ");
    // }

    // printf("]\np:  [");

    // for(uint8_t i = 0; i < jobs.size(); ++i) {
    //     printf("%3d", jobs[pi[i]].p);
    //     if(i != jobs.size() - 1) printf(", ");
    // }

    // printf("]\nq:  [");

    // for(uint8_t i = 0; i < jobs.size(); ++i) {
    //     printf("%3d", jobs[pi[i]].q);
    //     if(i != jobs.size() - 1) printf(", ");
    // }


    // log("Permutacja Naturalna", jobs, pi);


    pi = SchragePmtn(jobs);
    log("Kolejność po alg Schrage", jobs, pi);


    return 0;
}

