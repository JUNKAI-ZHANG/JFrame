#ifndef __BLOCK_QUEUE_H
#define __BLOCK_QUEUE_H

#include <chrono>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>

template <class T>
class BlockQueue {
   public:
    typedef std::deque<T> TQueue;

    // maxCapacity为-1，代表队列无最大限制
    explicit BlockQueue(const size_t iMaxCapacity = -1) : m_iMaxCapacity(iMaxCapacity) {
    }

    size_t Size() {
        std::unique_lock<std::mutex> lock(m_kMutex);
        return m_kTQueue.size();
    }

    void InsertFront(TQueue& kTQueue) {
        std::unique_lock<std::mutex> lock(m_kMutex);
        if (this->hasCapacity()) {
            while (m_kTQueue.size() + kTQueue.size() >= m_iMaxCapacity) {
                m_kNotFull.wait(lock);
            }
        }

        m_kTQueue.insert(m_kTQueue.begin(), kTQueue.begin(), kTQueue.end());
        m_kNotEmpty.notify_all();
    }

    void PushBack(T&& kT) {
        std::unique_lock<std::mutex> lock(m_kMutex);
        if (this->hasCapacity()) {
            while (m_kTQueue.size() >= m_iMaxCapacity) {
                m_kNotFull.wait(lock);
            }
        }

        m_kTQueue.push_back(std::move(kT));
        m_kNotEmpty.notify_all();
    }

    void PushFront(T&& kT) {
        std::unique_lock<std::mutex> lock(m_kMutex);
        if (hasCapacity()) {
            while (m_kTQueue.size() >= m_iMaxCapacity) {
                m_kNotFull.wait(lock);
            }
        }

        m_kTQueue.push_front(std::move(kT));
        m_kNotEmpty.notify_all();
    }

    void Swap(TQueue& kTQueue) {
        std::unique_lock<std::mutex> lock(m_kMutex);
        while (m_kTQueue.empty()) {
            m_kNotEmpty.wait(lock);
        }

        kTQueue.swap(m_kTQueue);

        m_kNotFull.notify_all();
    }

    void SwapTimeout(uint32_t iTimeoutMs, TQueue& kTQueue) {
        std::unique_lock<std::mutex> lock(m_kMutex);
        while (m_kTQueue.empty()) {
            if (m_kNotEmpty.wait_for(lock, std::chrono::milliseconds(iTimeoutMs)) == std::cv_status::timeout) {
                break;
            }
        }

        kTQueue.swap(m_kTQueue);

        m_kNotFull.notify_all();
    }

    T Pop() {
        std::unique_lock<std::mutex> lock(m_kMutex);
        while (m_kTQueue.empty()) {
            m_kNotEmpty.wait(lock);
        }

        T temp = *m_kTQueue.begin();
        m_kTQueue.pop_front();

        m_kNotFull.notify_all();

        return temp;
    }

    void PopTimeout(uint32_t iTimeoutMs, TQueue& listMsg) {
        std::unique_lock<std::mutex> lock(m_kMutex);
        while (m_kTQueue.empty()) {
            if (m_kNotEmpty.wait_for(lock, std::chrono::milliseconds(iTimeoutMs)) == std::cv_status::timeout) {
                break;
            }
        }

        if (!m_kTQueue.empty()) {
            listMsg.push_back(*m_kTQueue.begin());
            m_kTQueue.pop_front();
        }

        m_kNotFull.notify_all();

        return;
    }

    bool IsEmpty() {
        std::unique_lock<std::mutex> lock(m_kMutex);
        return m_kTQueue.empty();
    }

    bool IsFull() {
        std::unique_lock<std::mutex> lock(m_kMutex);
        if (!hasCapacity()) {
            return false;
        }

        return m_kTQueue.size() >= m_iMaxCapacity;
    }

   private:
    bool hasCapacity() const {
        return m_iMaxCapacity > 0;
    }

   private:
    TQueue m_kTQueue;

    const size_t m_iMaxCapacity;

    std::mutex m_kMutex;
    std::condition_variable m_kNotEmpty;
    std::condition_variable m_kNotFull;
};

#endif  //__BLOCK_QUEUE_H
