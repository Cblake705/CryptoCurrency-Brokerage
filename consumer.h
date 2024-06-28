//consumer.h
//made by christian blake
//red id: 824904815
#ifndef CONSUMER_H
#define CONSUMER_H

#include "tradecrypto.h" 
#include <pthread.h>

// Structure for passing arguments to consumer threads
struct ConsumerArgs {
    ConsumerType consumerType;  // Type of consumer (Blockchain X or Y)
    int sleepTime;              // Time to sleep simulating consumption delay
    SharedResources* shared;  // Pointer to shared resources
    unsigned int* consumed; // Pointer to the consumed count array
};

// Function implemented in consumer.cpp
void* consumer_function(void* arg);

#endif  // CONSUMER_H
