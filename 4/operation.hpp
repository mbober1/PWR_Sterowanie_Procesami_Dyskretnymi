#include <vector>

class Job
{
public:

    int number; // numer zadania
    int processingTime; //czas wykonywania
    int weight; //waga, kara
    int deadline; //deadline

    Job(int number);
    ~Job();
};

Job::Job(int number) : number(number) {}
Job::~Job() {}