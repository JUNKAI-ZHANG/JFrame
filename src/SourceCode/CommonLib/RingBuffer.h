#ifndef __RINGBUFFER_H_
#define __RINGBUFFER_H_

#include <stdint.h>

/*
 * @brief 环形缓存，优化服务器缓存读写，避免PopBuffer浪费服务器性能，同时简化Buffer的操作难度。
 */
class RingBuffer
{
public:
    /*
     * @brief default construct will distribute MAX_BUFFER_SIZE
     */
    RingBuffer();

    ~RingBuffer();

    /*
     * @brief 向缓存区里添加一段新的Buffer
     */
    bool AddBuffer(uint8_t *buffer, uint32_t size);

    /*
     * @brief 从缓存区里去掉一段Buffer
     */
    bool PopBuffer(uint32_t size);

    /*
     * @brief 需要手动释放!!
     */
    uint8_t *GetBuffer(uint32_t len);

    /*
     * @brief 得到缓存区剩余的容量
     */
    uint32_t GetRemain();

    /*
     * @brief 得到当前缓存区大小
     */
    uint32_t GetCapacity();

    uint8_t operator[](int id);

private:
    uint8_t *_m_buffer;
    uint32_t _m_begin;
    uint32_t _m_end;

    /* 剩余空间 */
    uint32_t _m_remain;
    /* 当前已使用 */
    uint32_t _m_capacity;

};

#endif // __RINGBUFFER_H_