#include <vector>

class job
{
public:
    int number, r, p, q; //numer, czas przygotowywania, czas wykonywania, czas stygniecia
    job(int number, int r, int p);
    job(int number);
    bool operator() (const job& A, const job& B);

    static int minR(std::vector<job> N);
    static int minQ(std::vector<job> N);
    static int minRit(std::vector<job> N);
    static int maxQ(std::vector<job> N);
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

int job::minR(std::vector<job> N) {
    int t = (1<<16);
    for(std::vector<job>::iterator it = N.begin(); it != N.end(); ++it) {
        t = min(t, it->r);
    }
    return t;
}

int job::minQ(std::vector<job> N) {
    int t = (1<<16);
    for(std::vector<job>::iterator it = N.begin(); it != N.end(); ++it) {
        t = min(t, it->q);
    }
    return t;
}

int job::minRit(std::vector<job> N) {
    int t = minR(N);
    for(int i = 0; i < N.size(); ++i) if(N[i].r == t) return i;
    return -1;
}

int job::maxQ(std::vector<job> N) {
    int t = 0;
    for(std::vector<job>::iterator it = N.begin(); it != N.end(); ++it) {
        if(it->q > N[t].q) t = it-N.begin();
    }
    return t;
}

bool job::operator() (const job& A, const job& B) {
    if (A.r < B.r) return !true;
    if (A.r > B.r) return !false;

    if (A.q > B.q) return !true;
    if (A.q < B.q) return !false;

    return !false; 
};