# Cryptocurrency Trading Simulation

## Project Overview
This project, part of a concurrent programming course, involved creating a simulation of a cryptocurrency trading system that handles concurrent trade requests for Bitcoin and Ethereum using producer-consumer patterns. The objective was to manage threading and synchronization issues effectively, ensuring thread-safe operations while processing numerous trade requests concurrently.

## Technologies Used
- **Programming Languages:** C++
- **Concurrency Tools:** POSIX Threads, Mutexes, Condition Variables
- **Development Environment:** Linux

## Project Details
1. **Concurrency Handling:**
   - Implemented producer and consumer threads to handle trade requests for Bitcoin and Ethereum.
   - Used mutexes and condition variables to manage access to a shared queue and control the rate of request production and consumption.
   - Ensured that the maximum number of Bitcoin requests did not exceed a predefined limit, avoiding overproduction and potential race conditions.

2. **Trade Request Processing:**
   - Producers generated trade requests based on predefined limits, pushing them into a shared queue.
   - Consumers processed these requests, simulating trade execution and updating system state accordingly.
   - Incorporated logging for each step to monitor the system's behavior and verify the correctness of concurrency mechanisms.

3. **Challenges and Solutions:**
   - Faced challenges with race conditions and deadlocks, particularly when integrating multiple producers and consumers.
   - Applied detailed synchronization techniques to ensure that threads operate without interference and maintain data integrity throughout the process.

## Results
The project successfully demonstrated the ability to handle complex concurrency scenarios in a high-stakes environment like cryptocurrency trading. It also highlighted the critical importance of proper thread synchronization to prevent data corruption and ensure system reliability.

## Conclusion
This simulation significantly improved my understanding of concurrent programming, particularly in the context of real-time financial systems. It sharpened my skills in applying advanced synchronization techniques and problem-solving under conditions that mimic real-world trading platforms.
