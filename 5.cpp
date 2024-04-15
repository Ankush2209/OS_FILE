#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore>

class ReadersWriters {
private:
    std::mutex mutex;
    sem_t write_mutex;
    int readers_count;

public:
    ReadersWriters() : readers_count(0) {
        sem_init(&write_mutex, 0, 1);
    }

    void start_read() {
        mutex.lock();
        readers_count++;
        if (readers_count == 1) {
            sem_wait(&write_mutex);
        }
        mutex.unlock();

        // Reading the shared resource
        std::cout << "Reader is reading" << std::endl;

        mutex.lock();
        readers_count--;
        if (readers_count == 0) {
            sem_post(&write_mutex);
        }
        mutex.unlock();
    }

    void start_write() {
        sem_wait(&write_mutex);

        // Writing to the shared resource
        std::cout << "Writer is writing" << std::endl;

        sem_post(&write_mutex);
    }
};

void reader_thread(ReadersWriters& rw, int id) {
    while (true) {
        // Reading
        rw.start_read();
        // Simulating some delay for reading
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void writer_thread(ReadersWriters& rw, int id) {
    while (true) {
        // Writing
        rw.start_write();
        // Simulating some delay for writing
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

int main() {
    ReadersWriters rw;

    // Creating reader threads
    std::thread readers[5];
    for (int i = 0; i < 5; ++i) {
        readers[i] = std::thread(reader_thread, std::ref(rw), i);
    }

    // Creating writer threads
    std::thread writers[2];
    for (int i = 0; i < 2; ++i) {
        writers[i] = std::thread(writer_thread, std::ref(rw), i);
    }

    // Joining threads
    for (int i = 0; i < 5; ++i) {
        readers[i].join();
    }
    for (int i = 0; i < 2; ++i) {
        writers[i].join();
    }

    return 0;
}
