#ifndef __BLOCK_QUEUE_H
#define __BLOCK_QUEUE_H

#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>

template <class T>
class BlockQueue {
   public:
    typedef std::unique_lock<std::mutex> TLock;
    typedef std::deque<T> TList;

    // maxCapacity为-1，代表队列无最大限制
    explicit BlockQueue(const size_t iMaxCapacity = -1) : m_iMaxCapacity(iMaxCapacity) {
    }

    size_t size() {
        TLock lock(m_kMutex);
        return m_list.size();
    }

    void insert_deque_front(TList& list) {
        TLock lock(m_kMutex);
        if (hasCapacity()) {
            while (m_list.size() + list.size() >= m_iMaxCapacity) {
                m_kNotFull.wait(lock);
            }
        }

        m_list.insert(m_list.begin(), list.begin(), list.end());
        m_kNotEmpty.notify_all();
    }

    void push_back(const T& item) {
        TLock lock(m_kMutex);
        if (hasCapacity()) {
            while (m_list.size() >= m_iMaxCapacity) {
                m_kNotFull.wait(lock);
            }
        }

        m_list.push_back(item);
        m_kNotEmpty.notify_all();
    }

    void push_front(const T& item) {
        TLock lock(m_kMutex);
        if (hasCapacity()) {
            while (m_list.size() >= m_iMaxCapacity) {
                m_kNotFull.wait(lock);
            }
        }

        m_list.push_front(item);
        m_kNotEmpty.notify_all();
    }

    void swap(TList& tlist) {
        TLock lock(m_kMutex);
        while (m_list.empty()) {
            m_kNotEmpty.wait(lock);
        }

        tlist.swap(m_list);

        m_kNotFull.notify_all();
    }

    void swap_timeout(uint32_t timeout_ms, TList& tlist) {
        TLock lock(m_kMutex);
        while (m_list.empty()) {
            if (m_kNotEmpty.wait_for(lock, std::chrono::milliseconds(timeout_ms)) == std::cv_status::timeout) {
                break;
            }
        }

        tlist.swap(m_list);

        m_kNotFull.notify_all();
    }

    T pop() {
        TLock lock(m_kMutex);
        while (m_list.empty()) {
            m_kNotEmpty.wait(lock);
        }

        T temp = *m_list.begin();
        m_list.pop_front();

        m_kNotFull.notify_all();

        return temp;
    }

    void pop_timeout(uint32_t iTimeoutMs, TList& listMsg) {
        TLock lock(m_kMutex);
        while (m_list.empty()) {
            if (m_kNotEmpty.wait_for(lock, std::chrono::milliseconds(iTimeoutMs)) == std::cv_status::timeout) {
                break;
            }
        }

        if (!m_list.empty()) {
            listMsg.push_back(*m_list.begin());
            m_list.pop_front();
        }

        m_kNotFull.notify_all();

        return;
    }

    bool empty() {
        TLock lock(m_kMutex);
        return m_list.empty();
    }

    bool full() {
        if (!hasCapacity()) {
            return false;
        }

        TLock lock(m_kMutex);
        return m_list.size() >= m_iMaxCapacity;
    }

   private:
    bool hasCapacity() const {
        return m_iMaxCapacity > 0;
    }

   private:
    TList m_list;

    const int m_iMaxCapacity;

    std::mutex m_kMutex;
    std::condition_variable m_kNotEmpty;
    std::condition_variable m_kNotFull;
};

#endif  //__BLOCK_QUEUE_H
