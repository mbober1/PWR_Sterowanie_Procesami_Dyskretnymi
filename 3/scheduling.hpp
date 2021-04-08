#include <vector>
#include "operation.hpp"
#include "RandomNumberGenerator.h"



Operation minP(std::vector<Operation> &N) {
    Operation po = N[0];
    for(Operation &op : N) {
        if(op.duration < po.duration) po = op;
    }
    return po;
}

void jonson(std::vector<Operation> &N, const int &n) {
    int l = 1;
    int k = n;

    while(!N.empty()) {
        Operation op = minP(N);
        
    }
}   


