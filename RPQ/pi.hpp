class Pi {
public:
    int s; // początek wykonywania zadania
    int c; // koniec wykonywania zadania
    int number; // numer zadania
    Pi(int s, int c, int number);
};

Pi::Pi(int s, int c, int number): s(s), c(c), number(number) {}
