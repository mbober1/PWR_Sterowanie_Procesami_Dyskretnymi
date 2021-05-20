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
    int last();
};

Job::Job() {}
Job::~Job() {}

void Job::addOperation(Operation tmp) {
    op.push_back(tmp);
}

int Job::last() {
    return this->op.size() - 1;
}