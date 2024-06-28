//consumer.cpp
//made by christian blake
//red id: 824904815
#include "consumer.h"
#include "report.h"
#include <chrono>
#include <thread>

//consumer function for processing trade requests
void* consumer_function(void* arg) {
    // Set void pointer to ConsumerArgs pointer for accessing args
    ConsumerArgs* consumerArgs = static_cast<ConsumerArgs*>(arg);
    //Retrieve consumertype X or Y
    ConsumerType type = consumerArgs->consumerType;
    //retrieve the sleep time
    int sleepTime = consumerArgs->sleepTime;
    //Access shared resources
    SharedResources* shared = consumerArgs->shared;

    // Initialize consumed count outside the loop
    unsigned int consumed[RequestTypeN] = {0};

    //main loop for consumer
    while (true) {
        //lock shared mutex before accessing
        pthread_mutex_lock(&shared->queueMutex);

        // Check if production is complete and the queue is empty
        if (shared->productionComplete && shared->requestQueue.empty()) {
            //unlock before break
            pthread_mutex_unlock(&shared->queueMutex);
            break; // Exit the loop and end the thread
        }
        //wait for request queue to be empty
        while (shared->requestQueue.empty()) {
            pthread_cond_wait(&shared->queueNotEmpty, &shared->queueMutex);
        }
        // Pop the request form queue
        TradeRequest request = shared->requestQueue.front();
        shared->requestQueue.pop();
        
        // Decrement Bitcoin request count if the consumed request is a Bitcoin request
        if (request.type == Bitcoin) {
            shared->bitcoinRequestCount--;
            if (shared->bitcoinRequestCount < 6) {
                pthread_cond_signal(&shared->bitcoinRequestLimit);
            }
        }

        // Update consumed count for the type of request consumed
        RequestType requestType = request.type;
        consumed[requestType]++;  
        consumerArgs->consumed[requestType]++;

        //Record request removal
        unsigned int inQueue = shared->requestQueue.size();
        report_request_removed(type, requestType, consumed, &inQueue);
        

        shared->allConsumed = true;
        //signal queue isn't full
        pthread_cond_signal(&shared->queueNotFull);
        //Unlock the mutex
        pthread_mutex_unlock(&shared->queueMutex);
        // Sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }

    return nullptr;
}