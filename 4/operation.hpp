#include <vector>

class Job
{
public:

    int processingTime; //czas wykonywania
    int weight; //waga, kara
    int deadline; //deadline

    Job();
    ~Job();
};

Job::Job() {}
Job::~Job() {}