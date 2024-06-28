//main.cpp
//made by christian blake
//red id: 824904815
//Side note, I used CentOS 7.9 on edoras servers using remote development
//Pretty similar to requirements, hopefully it works
#include <iostream>
#include <pthread.h>
#include <vector>
#include <queue>
#include "tradecrypto.h"
#include "producer.h"
#include "consumer.h"
#include "report.h"
#include <cstring>


int main(int argc, char *argv[]) {
    // Parse command line arguments
    int totalRequestLimit = 120; // Default # of trade req.
    int producerSleepTime = 0;   // Default sleep time for bitcoin
    int consumerSleepTimeX = 0;  // Default sleep time for X
    int consumerSleepTimeY = 0;  // Default sleep time for Y
    int ethereumProducerSleepTime = 0; // Default sleep time for Eth.
    const int maxQueueSize = 15;
    // initialize arrays to store counts
    unsigned int produced[RequestTypeN] = {0};
    unsigned int consumedX[RequestTypeN] = {0};
    unsigned int consumedY[RequestTypeN] = {0};

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            totalRequestLimit = std::atoi(argv[++i]);
        } else if (strcmp(argv[i], "-x") == 0 && i + 1 < argc) {
            consumerSleepTimeX = std::atoi(argv[++i]);
        } else if (strcmp(argv[i], "-y") == 0 && i + 1 < argc) {
            consumerSleepTimeY = std::atoi(argv[++i]);
        } else if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
            producerSleepTime = std::atoi(argv[++i]); // For Bitcoin producer
        } else if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            ethereumProducerSleepTime = std::atoi(argv[++i]); // For Ethereum producer
        }
    }
    // Initialize mutex and condition variables
    // Create an instance of SharedResources with the maximum queue size
    SharedResources sharedResources(maxQueueSize);
    pthread_mutex_init(&sharedResources.queueMutex, NULL);
    pthread_cond_init(&sharedResources.queueNotFull, NULL);
    pthread_cond_init(&sharedResources.queueNotEmpty, NULL);

    // Create producer and consumer arguments
    ProducerArgs bitcoinProducerArgs = {Bitcoin, producerSleepTime, totalRequestLimit, &sharedResources};
    ProducerArgs ethereumProducerArgs = {Ethereum, ethereumProducerSleepTime, totalRequestLimit, &sharedResources};
    ConsumerArgs blockchainXConsumerArgs = {BlockchainX, consumerSleepTimeX, &sharedResources};
    ConsumerArgs blockchainYConsumerArgs = {BlockchainY, consumerSleepTimeY, &sharedResources};
    bitcoinProducerArgs.produced = produced;
    ethereumProducerArgs.produced = produced;
    blockchainXConsumerArgs.consumed = consumedX;
    blockchainYConsumerArgs.consumed = consumedY;
    // Create threads
    pthread_t bitcoinProducerThread, ethereumProducerThread;
    pthread_t blockchainXConsumerThread, blockchainYConsumerThread;
    pthread_create(&bitcoinProducerThread, NULL, producer_function, &bitcoinProducerArgs);
    pthread_create(&ethereumProducerThread, NULL, producer_function, &ethereumProducerArgs);
    pthread_create(&blockchainXConsumerThread, NULL, consumer_function, &blockchainXConsumerArgs);
    pthread_create(&blockchainYConsumerThread, NULL, consumer_function, &blockchainYConsumerArgs);

    // Wait for producer threads finished
    pthread_join(bitcoinProducerThread, NULL);
    pthread_join(ethereumProducerThread, NULL);

    // Mechanism to signal consumer threads to exit
    sharedResources.productionComplete = true;
    // once all production is done and the queue is empty
    pthread_mutex_lock(&sharedResources.queueMutex);
    while (!sharedResources.allConsumed) {
        pthread_cond_wait(&sharedResources.consumptionComplete, &sharedResources.queueMutex);
    }
    pthread_mutex_unlock(&sharedResources.queueMutex);

    // Wait for consumer threads to finish
    pthread_join(blockchainXConsumerThread, NULL);

    pthread_join(blockchainYConsumerThread, NULL);

    // Clean up done in tradecrypto.h major error because originally
    // I did clean up right here. This wouldn't let my code exit properly
    // Ultimately keeping the main thread running
    // come back and set up final report
    unsigned int* consumed[ConsumerTypeN] = {consumedX, consumedY};
    report_production_history(produced, consumed);
    return 0;
}