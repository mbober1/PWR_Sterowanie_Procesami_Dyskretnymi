class Operation
{
public:

    int number; //numer zadania
    int duration; //czas trwania
    int end; //czas zakończenia

    Operation(int number, int duration);
    Operation();
    ~Operation();
};

Operation::Operation(int number, int duration) : number(number), duration(duration) {}
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