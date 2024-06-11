#ifndef __RINGBUFFER_H_
#define __RINGBUFFER_H_

#include <stdint.h>

/*
 * @brief 环形缓存，优化服务器缓存读写，避免PopBuffer浪费服务器性能，同时简化Buffer的操作难度。
 */
class RingBuffer {
   public:
    /*
     * @brief default construct will distribute MAX_BUFFER_SIZE
     */
    RingBuffer() {
        _m_buffer = new char[MAX_BUFFER_SIZE];
        _m_begin = _m_end = _m_capacity = 0;
        _m_remain = MAX_BUFFER_SIZE;
    }

    ~RingBuffer() {
        _m_begin = _m_end = _m_capacity = 0;
        _m_remain = MAX_BUFFER_SIZE;
        delete _m_buffer;
    }

    /*
     * @brief 向缓存区里添加一段新的Buffer
     */
    bool AddBuffer(char *buffer, uint32_t size) {
        if (size > _m_remain) {
            return false;
        }

        for (uint32_t i = 0; i < size; i++) {
            _m_buffer[_m_end] = buffer[i];
            _m_end = (_m_end + 1) % MAX_BUFFER_SIZE;
        }

        _m_capacity += size;
        _m_remain -= size;

        return true;
    }

    /*
     * @brief 从缓存区里去掉一段Buffer
     */
    bool PopBuffer(uint32_t size) {
        if (size > _m_capacity) {
            return false;
        }

        _m_begin = (_m_begin + size) % MAX_BUFFER_SIZE;

        _m_capacity -= size;
        _m_remain += size;
        return true;
    }

    /*
     * @brief 需要手动释放!!
     */
    char *GetBuffer(uint32_t len) {
        uint32_t start = _m_begin;
        char *ret = new char[len];

        for (uint32_t i = 0; i < len; i++) {
            ret[i] = _m_buffer[start];
            start = (start + 1) % MAX_BUFFER_SIZE;
        }

        return ret;
    }

    /*
     * @brief 得到缓存区剩余的容量
     */
    uint32_t GetRemain() {
        return _m_remain;
    }

    /*
     * @brief 得到当前缓存区大小
     */
    uint32_t GetCapacity() {
        return _m_capacity;
    }

    char operator[](int id) {
        if (id < 0 || id >= MAX_BUFFER_SIZE) {
            return 0;
        }
        return _m_buffer[id];
    }

   private:
    char *_m_buffer;
    uint32_t _m_begin;
    uint32_t _m_end;

    /* 剩余空间 */
    uint32_t _m_remain;
    /* 当前已使用 */
    uint32_t _m_capacity;

   private:
    static const int32_t MAX_BUFFER_SIZE = 1024 * 1024;
};

#endif  // __RINGBUFFER_H_