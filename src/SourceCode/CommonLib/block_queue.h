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
    explicit BlockQueue(const int maxCapacity = -1) : m_maxCapacity(maxCapacity) {
    }

    size_t size() {
        TLock lock(m_mutex);
        return m_list.size();
    }

    void insert_deque_front(TList& list) {
        TLock lock(m_mutex);
        if (hasCapacity()) {
            while (m_list.size() + list.size() >= (size_t)m_maxCapacity) {
                m_notFull.wait(lock);
            }
        }

        m_list.insert(m_list.begin(), list.begin(), list.end());
        m_notEmpty.notify_all();
    }

    void push_back(const T& item) {
        TLock lock(m_mutex);
        if (hasCapacity()) {
            while (m_list.size() >= (size_t)m_maxCapacity) {
                m_notFull.wait(lock);
            }
        }

        m_list.push_back(item);
        m_notEmpty.notify_all();
    }

    void push_front(const T& item) {
        TLock lock(m_mutex);
        if (hasCapacity()) {
            while (m_list.size() >= (size_t)m_maxCapacity) {
                m_notFull.wait(lock);
            }
        }

        m_list.push_front(item);
        m_notEmpty.notify_all();
    }

    void swap(std::deque<T>& tlist) {
        TLock lock(m_mutex);
        while (m_list.empty()) {
            m_notEmpty.wait(lock);
        }

        tlist.swap(m_list);

        m_notFull.notify_all();
    }

    void swap_timeout(uint32_t timeout_ms, std::deque<T>& tlist) {
        TLock lock(m_mutex);
        while (m_list.empty()) {
            if (m_notEmpty.wait_for(lock, std::chrono::milliseconds(timeout_ms)) == std::cv_status::timeout) {
                break;
            }
        }

        tlist.swap(m_list);

        m_notFull.notify_all();
    }

    void swap_part_of_timeout(uint32_t timeout_ms, std::deque<T>& tlist, int part_of) {
        TLock lock(m_mutex);
        while (m_list.empty()) {
            if (m_notEmpty.wait_for(lock, std::chrono::milliseconds(timeout_ms)) == std::cv_status::timeout) {
                break;
            }
        }

        if (m_list.size() <= part_of) {
            tlist.swap(m_list);
        } else {
            while (!m_list.empty() && tlist.size() < part_of) {
                tlist.push_back(*m_list.begin());
                m_list.pop_front();
            }
        }

        m_notFull.notify_all();
    }

    T pop() {
        TLock lock(m_mutex);
        while (m_list.empty()) {
            m_notEmpty.wait(lock);
        }

        T temp = *m_list.begin();
        m_list.pop_front();

        m_notFull.notify_all();

        return temp;
    }

    void pop_timeout(uint32_t timeout_ms, std::deque<T>& tlist) {
        TLock lock(m_mutex);
        while (m_list.empty()) {
            if (m_notEmpty.wait_for(lock, std::chrono::milliseconds(timeout_ms)) == std::cv_status::timeout) {
                break;
            }
        }

        if (!m_list.empty()) {
            tlist.push_back(*m_list.begin());
            m_list.pop_front();
        }

        m_notFull.notify_all();

        return;
    }

    bool empty() {
        TLock lock(m_mutex);
        return m_list.empty();
    }

    bool full() {
        if (!hasCapacity()) {
            return false;
        }

        TLock lock(m_mutex);
        return m_list.size() >= m_maxCapacity;
    }

   private:
    bool hasCapacity() const {
        return m_maxCapacity > 0;
    }

    TList m_list;

    const int m_maxCapacity;

    std::mutex m_mutex;
    std::condition_variable m_notEmpty;
    std::condition_variable m_notFull;
};

#endif  //__BLOCK_QUEUE_H
