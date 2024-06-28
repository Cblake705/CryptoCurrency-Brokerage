//producer.h
//made by christian blake
//red id: 824904815
#ifndef PRODUCER_H
#define PRODUCER_H

#include "tradecrypto.h"

// Structure for passing arguments to producer threads
struct ProducerArgs {
    RequestType requestType;  // type of request to produce
    int sleepTime;            // Sleep time
    int productionLimit;      // limit for requests total
    SharedResources* shared;  // shared resources pointer
    unsigned int* produced; // pointer to the produced count array
};

void* producer_function(void* arg);

#endif  // PRODUCER_H