#include <vector>

class Job
{
public:

    int number; // numer zadania
    int processingTime; //czas wykonywania
    int weight; //waga, kara
    int deadline; //deadline

    Job(int number);
    Job(int number, int processingTime, int weight, int deadline);

    ~Job();
};

Job::Job(int number) : number(number) {}

Job::Job(int number, int processingTime, int weight, int deadline) :
number(number),
weight(weight),
processingTime(processingTime), 
deadline(deadline) {}

Job::~Job() {}