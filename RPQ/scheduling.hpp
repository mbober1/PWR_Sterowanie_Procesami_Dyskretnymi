#include "job.hpp"
#include <stdint.h>
#include "pi.hpp"
#include <queue>

int cmax(std::vector<job> &jobs, const std::vector<Pi> &pi, int* j = nullptr) {
    int c = 0;
    int idx = 0;
    for (int i = 0; i < pi.size(); ++i) {
        int tmp = pi[i].c + jobs[pi[i].number].q;
        if (tmp > c) {
            c = tmp;
            idx = i;
        } 
    }
    if(j != nullptr) *j = idx;
    return c;
}

int calcA(std::vector<job> &jobs, const std::vector<Pi> &pi, int b) {
    int min = (1<<16);
    int minj = 0;
    for(int j = 0; j < pi.size(); ++j) {
        int sum = 0;
        for(int i = j; i < b; ++i) sum += jobs[pi[i].number].p;
        int zmienna = jobs[pi[j].number].r + sum + jobs[pi[b].number].q;
        if(zmienna < min) {
            min = zmienna;
            minj = j;
        }
    }
    return minj;
}

int calcC(std::vector<job> &jobs, const std::vector<Pi> &pi, int a, int b) {
    int idx = -1;
    for(int j = a; j < b; ++j ) {
        // printf("%d vs %d | %d > %d\n", jobs[pi[j].number].q, jobs[pi[b].number].q, j, idx);
        if(jobs[pi[j].number].q < jobs[pi[b].number].q && j>idx) idx = j;
    }
    return idx;
}


std::vector<Pi> SchragePmtn(const std::vector<job> &jobs) {
    std::vector<Pi> pi; //lista wykonywania
    std::vector<job> G; //zbiór zadań gotowych do realizacji
    std::vector<job> N; //zbiór zadań nieuszeregowanych

    // Inicjalizacja listy zadań nieuszeregowanych
    for(uint8_t i = 0; i<jobs.size(); ++i) N.push_back(jobs[i]);

    // Ustawiamy aktualny czas na początek najwcześniejszego zadania
    int t = job::minR(N);

    // Działamy dopóki zostały zadania
    while (!G.empty() || !N.empty()) {
        // Jeżeli jest nieuszeregowane zadanie które jest już gotowe...
        while (!N.empty() && job::minR(N) <= t) {
            // to przerzucamy je do G
            G.push_back(N[job::minRit(N)]);
            N.erase(N.begin() + job::minRit(N));
        }

        // Jeśli nie ma gotowych zadań to przesuwamy czas do najbliższego zadania
        if (G.empty()) t = job::minR(N);
        // w przeciwnym wypadku...
        else {
            // szukamy zadania najdluzej stygnącego
            int j = job::maxQ(G);
            // zwiększamy czas... 
            
            pi.emplace_back(t, t+G[j].p, G[j].number);
            // printf("[%d] S: %d, C: %d\n", pi[pi.size() - 1].number, pi[pi.size() - 1].s, pi[pi.size() - 1].c);
            t += G[j].p;

            // Jeżeli jest nieuszeregowane zadanie które dlużej stygnie
            if (!N.empty() && job::minR(N) <= t && N[job::minRit(N)].q > G[j].q) {
                // printf("Zadanie %d przerywa zadanie %d!\n", N[0].number+1, G[j].number+1);
                auto minerit = job::minRit(N);
                auto miner = N[minerit].r;
                // Ustawiamy aktualnemu zadaniu nowy czas trwania (pomniejszony o to co już wykonał)
                G[j].p = t - miner;

                // Cofamy się w czasie
                t = miner;

                // Aktualizacja czasu skonczenia zadania
                // printf("Zmiana [%d] C: %d -> %d\n", pi[pi.size() - 1].number, pi[pi.size() - 1].c, t);
                pi[pi.size() - 1].c = t;

                // Przenosimy aktualne zadanie do nieuszeregowanych i usuwamy to zadanie
                N.push_back(G[j]);
                G.erase(G.begin() + j);

                // Dodajemy nowe zadanie do gotowych do wykonania i usuwamy je z nieuszeregowanych
                G.push_back(N[minerit]);
                N.erase(N.begin() + minerit);

                j = job::maxQ(G);
            }
            // i usuwamy to zadanie
            else G.erase(G.begin() + j);
        }
    }
    return pi;
}



std::vector<Pi> Schrage(const std::vector<job> &jobs) {
    std::vector<Pi> pi; //lista wykonywania
    std::vector<job> G; //zbiór zadań gotowych do realizacji
    std::vector<job> N; //zbiór zadań nieuszeregowanych

    // Inicjalizacja listy zadań nieuszeregowanych
    for(uint8_t i = 0; i<jobs.size(); ++i) N.push_back(jobs[i]);

    // Ustawiamy aktualny czas na początek najwcześniejszego zadania
    int t = job::minR(N);

    // Działamy dopóki zostały zadania
    while (!G.empty() || !N.empty()) {
        // Jeżeli jest nieuszeregowane zadanie które jest już gotowe...
        while (!N.empty() && job::minR(N) <= t) {
            // to przerzucamy je do G
            G.push_back(N[job::minRit(N)]);
            N.erase(N.begin() + job::minRit(N));
        }

        // Jeśli nie ma gotowych zadań to przesuwamy czas do najbliższego zadania
        if (G.empty()) t = job::minR(N);
        // w przeciwnym wypadku...
        else {
            // szukamy zadania najdluzej stygnącego
            int j = job::maxQ(G);
            // zwiększamy czas... 
            // printf("Zaczynamy zadanie %d\n", G[j].number+1);
            pi.emplace_back(t, t+G[j].p, G[j].number);
            // printf("[%d] S: %d, C: %d\n", pi[pi.size() - 1].number, pi[pi.size() - 1].s, pi[pi.size() - 1].c);
            t += G[j].p;

            // i usuwamy to zadanie
            G.erase(G.begin() + j);
        }
    }
    return pi;
}


class NCompare {
public:
    bool operator() (const job& A, const job& B) {
        if (A.r < B.r) return false;
        if (A.r > B.r) return true;

        if (A.q > B.q) return false;
        if (A.q < B.q) return true;

        return true; 
    };
};

class GCompare {
public:
    bool operator() (const job& A, const job& B) {
        return A.q < B.q;
    };
};

std::vector<Pi> SchrageHeap(std::vector<job> &jobs) {
    std::vector<Pi> pi; //lista wykonywania
    std::priority_queue<job, std::vector<job>, GCompare> G; //zbiór zadań gotowych do realizacji
    std::priority_queue<job, std::vector<job>, NCompare> N(jobs.begin(), jobs.end()); //zbiór zadań nieuszeregowanych

    // Ustawiamy aktualny czas na początek najwcześniejszego zadania
    int t = N.top().r;

    // Działamy dopóki zostały 
    while (!G.empty() || !N.empty()) {
        // Jeżeli jest nieuszeregowane zadanie które jest już gotowe...
        while (!N.empty() && N.top().r <= t) {
            // to przerzucamy je do G
            G.push(N.top());
            N.pop();
        }

        // Jeśli nie ma gotowych zadań to przesuwamy czas do najbliższego zadania
        if (G.empty()) t = N.top().r;
        // w przeciwnym wypadku...
        else {
            // szukamy zadania najdluzej stygnącego
            auto job = G.top();
            // zwiększamy czas... 
            pi.emplace_back(t, t+job.p, job.number);
            t += job.p;

            // i usuwamy to zadanie
            G.pop();
        }
    }
    return pi;
}


std::vector<Pi> Carlier(std::vector<job> jobs, std::vector<Pi> pi2 = {}, int UB = INFINITY) {
    std::vector<Pi> pi = Schrage(jobs);
    int b;
    int U = cmax(jobs, pi, &b);

    if(U < UB) {
        UB = U;
        pi2 = pi;
    }

    int a = calcA(jobs, pi, b);
    int c = calcC(jobs, pi, a, b);

    if(c == -1) return pi2;

    std::vector<job> K;
    for(int i = c+1; i <= b; ++i) K.push_back(jobs[pi[i].number]);

    int r2 = job::minR(K);
    int q2 = job::minQ(K);
    int p2 = 0;
    for(int i = 0; i < K.size(); ++i) p2 += K[i].p;

    auto r_bcp = jobs[pi[c].number].r;
    jobs[pi[c].number].r = max(jobs[pi[c].number].r, r2 + p2);

    int LB = cmax(jobs, SchragePmtn(jobs));
    if(LB < UB) {
        auto x = Carlier(jobs, pi2, UB);
        if (!x.empty()) return x;
    }
    jobs[pi[c].number].r = r_bcp;

    auto q_bcp = jobs[pi[c].number].q;
    jobs[pi[c].number].q = max(jobs[pi[c].number].q, q2 + p2);

    LB = cmax(jobs, SchragePmtn(jobs));
    
    if(LB < UB) {
        auto x = Carlier(jobs, pi2, UB);
        if (!x.empty()) return x;
    }

    jobs[pi[c].number].q = q_bcp;

    std::vector<Pi> nul = SchragePmtn(jobs);
    return nul;
}