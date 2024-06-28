// producer.cpp
//made by christian blake
//red id: 824904815
#include "producer.h"
#include "report.h"
#include <chrono>
#include <thread>

//executed by producer thread
//generates and adds trade requests to queue
void* producer_function(void* arg) {
    //Cast args for access
    ProducerArgs* producerArgs = static_cast<ProducerArgs*>(arg);
    //type of trade request(bit or eth)
    RequestType type = producerArgs->requestType;
    //Sleep time simulate
    int sleepTime = producerArgs->sleepTime;
    // Access shared resources
    SharedResources* shared = producerArgs->shared;

    while (true) {
        //lock mutex for exclusive access
        pthread_mutex_lock(&shared->queueMutex);

        // Check if the production limit has been reached
        if (shared->totalRequestsProduced >= producerArgs->productionLimit) {
            //unlock mutex
            pthread_mutex_unlock(&shared->queueMutex);
            break;
        }

        // Prevent more than 6 bitcoin requests
        if (type == Bitcoin && shared->bitcoinRequestCount >= 6) {
            pthread_cond_wait(&shared->bitcoinRequestLimit, &shared->queueMutex);
        }

        // Wait for queue to free up
        while (shared->requestQueue.size() >= static_cast<size_t>(shared->maxQueueSize)) {
            pthread_cond_wait(&shared->queueNotFull, &shared->queueMutex);
        }
        // Create and add a new TradeRequest of given type
        TradeRequest request(type);  
        shared->requestQueue.push(request);
        shared->totalRequestsProduced++;
        producerArgs->produced[type]++;
        //increment for bitcoin count
        if (type == Bitcoin) {
            shared->bitcoinRequestCount++;
        }

        // Report the request addition
        unsigned int inQueue = shared->requestQueue.size();
        report_request_added(type, producerArgs->produced, &inQueue);

        pthread_cond_signal(&shared->queueNotEmpty);
        pthread_mutex_unlock(&shared->queueMutex);

        // Sleep to simulate production time
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }

    return nullptr;
}
