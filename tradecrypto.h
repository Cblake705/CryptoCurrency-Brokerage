//tradecrypto.h
//made by christian blake
//red id: 824904815
#ifndef TRADECRYPTO_H
#define TRADECRYPTO_H
#include <queue>
#include <pthread.h>
/*
* Arrays with producer and consumer names
* These can be indexed with the enumerated types below
* and are defined in log.c
*/
extern const char *producerNames[];
extern const char *producerAbbrevs[];
extern const char *consumerNames[];
/*
* Enumerated types to be used by the producers and consumers
* These are expected in the input/output functions (log.h)
* should be useful in your producer and consumer code.
*/
/**
* The broker can hold up to a maximum of 15 unconsumed requests
* in its request queue at any given time.
* e. As a Bitcoin transaction costs more fee,
* there can be no more than 6 transaction requests for
* Bitcoins in the broker request queue at any given time.
*/
/*
* Two trade request services (producers of requests) are offered:
* one for publishing (producing) a bitcoin trader request,
* one for publishing (producing) an ethereum trader request.
* Each trade request service (producer) only publishes (produces)
* one type of requests,
* so RequestType is synomonous with the producer type
*/
typedef enum Requests {
Bitcoin = 0, // Bitcoin trade request
Ethereum = 1, // Ethereum trade request
RequestTypeN = 2, // number of trade request types
} RequestType;
/*
* Two trade transaction services (consumers of requests) are available using
* different block chains
* one uses blockchain X
* one uses blockchain Y
* Requests are taken off from the broker request queue (by consumers)
* in the order that they are published (produced).
*/
typedef enum Consumers {
BlockchainX = 0, // process request transaction on blockchain X
BlockchainY = 1, // process request transaction on blockchain Y
ConsumerTypeN = 2, // Number of consumers
} ConsumerType;

struct TradeRequest {
    RequestType type;  // Type of the request: Bitcoin or Ethereum
    // Additional fields can be added here, such as amount, trader ID, etc.
    // For simplicity, I'm only including the type in this example.

    TradeRequest(RequestType requestType) : type(requestType) {}
};


struct SharedResources {
    std::queue<TradeRequest> requestQueue;  // Queue for trade requests
    pthread_mutex_t queueMutex;             // Mutex for keeping access to the queue organized
    pthread_cond_t queueNotFull;            // Condition variable to signal that the queue is not full
    pthread_cond_t queueNotEmpty;           // Condition variable to signal that the queue is not empty
    pthread_cond_t bitcoinRequestLimit;    // New condition variable for Bitcoin request limit
    int totalRequestsProduced;              // Total number of requests produced
    int maxQueueSize;                       // Maximum size of the request queue
    bool productionComplete;                // Flag to indicate when production is completed
    bool allConsumed;                          // New flag
    int bitcoinRequestCount;  // New member to track the number of Bitcoin requests
    
    pthread_cond_t consumptionComplete;
    //constructor
    SharedResources(int maxQueueSize)
        : totalRequestsProduced(0), maxQueueSize(maxQueueSize), productionComplete(false), allConsumed(false), bitcoinRequestCount(0) {
        pthread_mutex_init(&queueMutex, NULL);
        pthread_cond_init(&queueNotFull, NULL);
        pthread_cond_init(&queueNotEmpty, NULL);
        pthread_cond_init(&consumptionComplete, NULL);  
        allConsumed = false;
        pthread_cond_init(&bitcoinRequestLimit, NULL); 
    }
    //destructor
    ~SharedResources() {
        pthread_mutex_destroy(&queueMutex);
        pthread_cond_destroy(&queueNotFull);
        pthread_cond_destroy(&queueNotEmpty);
        pthread_cond_destroy(&consumptionComplete);
        pthread_cond_destroy(&bitcoinRequestLimit); // Destroy the new condition variable
    }
};
#endif