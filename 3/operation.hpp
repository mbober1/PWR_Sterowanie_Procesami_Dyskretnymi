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
    Operation op[2];
    Job(Operation op1, Operation op2);
    ~Job();
};


Job::Job(Operation op1, Operation op2) {
    op[1] = op1;
    op[0] = op2;
}

Job::~Job() {}
