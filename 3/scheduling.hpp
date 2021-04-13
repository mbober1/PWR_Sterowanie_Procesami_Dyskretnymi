#include <vector>
#include "operation.hpp"
#include "RandomNumberGenerator.h"
#include <stdio.h>



Job minP(std::vector<Job> &N, int *it) {
    Job minJob = N[0];
    *it = 0;

    for(int i = 0; i < N.size(); ++i) {
        if(N[i].op[0].duration < minJob.op[0].duration && N[i].op[0].duration < minJob.op[1].duration) {
            minJob = N[i];
            *it = i;
        } 
        if(N[i].op[1].duration < minJob.op[0].duration && N[i].op[1].duration < minJob.op[1].duration) {
            minJob = N[i];
            *it = i;
        }
    }
    return minJob;
}

std::vector<Job> jonson(std::vector<Job> N) {
    int l = 0;
    int k = N.size() - 1;
    std::vector<Job> Pi;
    Pi.resize(N.size());

    while(!N.empty()) {
        int it;
        Job minJob = minP(N, &it);
        if(minJob.op[0].duration < minJob.op[1].duration) {
            Pi.at(l++) = minJob;
        } else {
            Pi.at(k--) = minJob;
        }
        fflush(stdout);
        N.erase(N.begin() + it);
    }

    return Pi;
}   
