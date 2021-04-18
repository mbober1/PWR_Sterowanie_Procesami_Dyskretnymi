class Operation
{
public:

    int number; //numer zadania
    int start; //czas rozpoczęcia
    int duration; //czas trwania
    int machine; //numer maszyny

    Operation(int number, int start, int duration, int machine);
    Operation();
    ~Operation();
};

Operation::Operation(int number, int start, int duration, int machine) 
: number(number), start(start), duration(duration), machine(machine) {}


Operation::Operation() {}
Operation::~Operation() {}


class Job
{
public:
    std::vector<Operation>op;
    Job();
    ~Job();
    void addOperation(Operation tmp);
};

Job::Job() {}
Job::~Job() {}

void Job::addOperation(Operation tmp) {
    op.push_back(tmp);
}