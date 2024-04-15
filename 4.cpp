#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

using namespace std;

#define NUM_THREADS 2

atomic<int> turn; // determines the turn of the process to enter the critical section
atomic<bool> flag[NUM_THREADS]; // Flags to show interest in the critical section

void process(int id) {
    int other = 1 - id;

    while (true) {
        // show interest in the critical section
        flag[id] = true;

        // Set turn to the other process
        turn = other;

        // Wait until it is your turn and the other process is not interested
        while (turn == other && flag[other].load()) {
            // Busy wait
            std::this_thread::yield(); 
        }

        // Critical section
        cout << "Process " << id << " is in the critical section" << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // critical section code

        // End of critical section
        cout << "Process " << id << " is leaving the critical section" << endl;

        // Reset flag showing interest
        flag[id] = false;

        // Non-critical section
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); 
    }
}

int main() {
    // Initialize flags and turn
    flag[0] = flag[1] = false;
    turn = 0;

    // Create threads
    vector<thread> threads;
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(process, i);
    }

    // Join threads
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
