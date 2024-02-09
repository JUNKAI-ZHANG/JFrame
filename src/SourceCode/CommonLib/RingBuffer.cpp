#ifndef __RINGBUFFER_CPP_
#define __RINGBUFFER_CPP_

#include <stdint.h>
#include <string.h>
#include <iostream>
#include <memory>

#include "./RingBuffer.h"
#include "../Common_Define.h"

RingBuffer::RingBuffer() {
    _m_buffer = new uint8_t[MAX_BUFFER_SIZE];
    _m_begin = _m_end = _m_capacity = 0;
    _m_remain = MAX_BUFFER_SIZE;
}

RingBuffer::~RingBuffer() {
    _m_begin = _m_end = _m_capacity = 0;
    _m_remain = MAX_BUFFER_SIZE;
    delete _m_buffer;
}

bool RingBuffer::AddBuffer(uint8_t *buffer, uint32_t size) {
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

bool RingBuffer::PopBuffer(uint32_t size) {
    if (size > _m_capacity) {
        return false;
    }

    _m_begin = (_m_begin + size) % MAX_BUFFER_SIZE;

    _m_capacity -= size;
    _m_remain += size;
    return true;
}

uint8_t *RingBuffer::GetBuffer(uint32_t len) {
    uint32_t start = _m_begin;
    uint8_t *ret = new uint8_t[len];

    for (uint32_t i = 0; i < len; i++) {
        ret[i] = _m_buffer[start];
        start = (start + 1) % MAX_BUFFER_SIZE;
    }

    return ret;
}

uint32_t RingBuffer::GetRemain() {
    return _m_remain;
}

uint32_t RingBuffer::GetCapacity() {
    return _m_capacity;
}

uint8_t RingBuffer::operator[](int id) {
    if (id < 0 || id >= MAX_BUFFER_SIZE)
        return 0;
    return _m_buffer[id];
}

#endif // __RINGBUFFER_CPP_