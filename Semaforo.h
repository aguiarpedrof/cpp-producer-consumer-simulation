#ifndef SEMAFORO_H
#define SEMAFORO_H

#include <mutex>
#include <condition_variable>

class Semaforo {
private:
    std::mutex mutex_;
    std::condition_variable cv_;
    int count_;

public:
    explicit Semaforo(int count = 0) : count_(count) {}

    void acquire() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]() { return count_ > 0; });
        --count_;
    }

    void release() {
        std::lock_guard<std::mutex> lock(mutex_);
        ++count_;
        cv_.notify_one();
    }

    bool try_acquire() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (count_ > 0) {
            --count_;
            return true;
        }
        return false;
    }
};

#endif
