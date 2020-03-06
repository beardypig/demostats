#include <vector>
#include <iostream>
#include "dstream.h"

const uint32_t hl2demo::dstream::s_nMaskTable[33] = {
        0,
        ( 1u << 1u ) - 1,
        ( 1u << 2u ) - 1,
        ( 1u << 3u ) - 1,
        ( 1u << 4u ) - 1,
        ( 1u << 5u ) - 1,
        ( 1u << 6u ) - 1,
        ( 1u << 7u ) - 1,
        ( 1u << 8u ) - 1,
        ( 1u << 9u ) - 1,
        ( 1u << 10u ) - 1,
        ( 1u << 11u ) - 1,
        ( 1u << 12u ) - 1,
        ( 1u << 13u ) - 1,
        ( 1u << 14u ) - 1,
        ( 1u << 15u ) - 1,
        ( 1u << 16u ) - 1,
        ( 1u << 17u ) - 1,
        ( 1u << 18u ) - 1,
        ( 1u << 19u ) - 1,
        ( 1u << 20u ) - 1,
        ( 1u << 21u ) - 1,
        ( 1u << 22u ) - 1,
        ( 1u << 23u ) - 1,
        ( 1u << 24u ) - 1,
        ( 1u << 25u ) - 1,
        ( 1u << 26u ) - 1,
        ( 1u << 27u ) - 1,
        ( 1u << 28u ) - 1,
        ( 1u << 29u ) - 1,
        ( 1u << 30u ) - 1,
        0x7fffffffu,
        0xffffffffu,
};

hl2demo::dstream::dstream(std::istream *io) : io(io) {
    init();
}

hl2demo::dstream::dstream(const std::string &data) : io_str(data), io(&io_str) {
    init();
}

bool hl2demo::dstream::is_eof() {
    if (m_bits_available > 0) {
        return false;
    }
    char t;
    io->get(t);
    if (io->eof()) {
        io->clear();
        return true;
    } else {
        io->unget();
        return false;
    }
}

void hl2demo::dstream::seekg(uint64_t pos) {
    io->seekg(pos);
}

void hl2demo::dstream::seekg(uint64_t pos, std::ios_base::seekdir dir) {
    io->seekg(pos, dir);
}

uint64_t hl2demo::dstream::tellg() {
    return io->tellg();
}

uint64_t hl2demo::dstream::size() {
    if (!m_has_size) {
        auto cur_pos = io->tellg();
        io->seekg(0, std::ios::end);  // seek to end
        m_size = io->tellg();
        io->seekg(cur_pos);  // restore save seek
    }
    return m_size;
}

uint64_t hl2demo::dstream::ulong(uint32_t nbits) {
    if ( m_bits_available >= nbits )
    {
        unsigned int nRet = m_bits & s_nMaskTable[ nbits ];
        m_bits_available -= nbits;
        if ( m_bits_available )
        {
            m_bits >>= nbits;
        }
        return nRet;
    }
    else
    {
        // need to merge words
        unsigned int nRet = m_bits;
        nbits -= m_bits_available;
        int new_bits_available = next_dword();
        nRet |= ( ( m_bits & s_nMaskTable[ nbits ] ) << m_bits_available );
        m_bits_available = new_bits_available - nbits;
        m_bits >>= nbits;
        return nRet;
    }
}

uint8_t hl2demo::dstream::byte() {
    return static_cast<uint8_t>(ulong(8));
}

void hl2demo::dstream::read(char *buf, size_t len) {
    assert(len > 0);
    if ((m_bits_available % 8) == 0) {
        int i = 0;
        while (m_bits_available > 0 && i < len) {
            buf[i++] = m_bits & 0xffu;
            m_bits >>= 8;
            m_bits_available -= 8;
        }
        io->read(&buf[i], len-i);
    } else {
        for (int i = 0; i < len; ++i) {
            char c = byte();
            buf[i] = c;
        }
    }
}

int16_t hl2demo::dstream::s2be() {
    int16_t t;
    read(reinterpret_cast<char *>(&t), 2);
#if __BYTE_ORDER == __LITTLE_ENDIAN
    t = bswap_16(t);
#endif
    return t;
}

int32_t hl2demo::dstream::s4be() {
    int32_t t;
    read(reinterpret_cast<char *>(&t), 4);
#if __BYTE_ORDER == __LITTLE_ENDIAN
    t = bswap_32(t);
#endif
    return t;
}

int64_t hl2demo::dstream::s8be() {
    int64_t t;
    read(reinterpret_cast<char *>(&t), 8);
#if __BYTE_ORDER == __LITTLE_ENDIAN
    t = bswap_64(t);
#endif
    return t;
}

int16_t hl2demo::dstream::s2le() {
    int16_t t;
    read(reinterpret_cast<char *>(&t), 2);
#if __BYTE_ORDER == __BIG_ENDIAN
    t = bswap_16(t);
#endif
    return t;
}

int32_t hl2demo::dstream::s4le() {
    int32_t t;
    read(reinterpret_cast<char *>(&t), 4);
#if __BYTE_ORDER == __BIG_ENDIAN
    t = bswap_32(t);
#endif
    return t;
}

int64_t hl2demo::dstream::s8le() {
    int64_t t;
    read(reinterpret_cast<char *>(&t), 8);
#if __BYTE_ORDER == __BIG_ENDIAN
    t = bswap_64(t);
#endif
    return t;
}


bool hl2demo::dstream::compare_fixed_string(const char *expected, size_t length) {
    char *actual = new char[length];
    read(actual, length);
    return memcmp(expected, actual, length);
}

std::string hl2demo::dstream::read(size_t len) {
    auto buf = new char[len];
    read(buf, len);
    auto r = std::string();
    r.assign(buf, len);
    delete[] buf;
    return r;
}

std::string hl2demo::dstream::bytes_terminate(std::string src, char terminator, bool include) {
    auto found = src.find(terminator);
    if (found < src.length()) {
        return src.substr(0, found + ((include) ? 1 : 0));
    } else {
        return src;
    }
}

float hl2demo::dstream::f4le() {
    uint32_t t;
    read(reinterpret_cast<char *>(&t), 4);
#if __BYTE_ORDER == __BIG_ENDIAN
    t = bswap_32(t);
#endif
    return reinterpret_cast<float&>(t);
}

int32_t hl2demo::dstream::next_dword() {
    m_bits = 0;
    int new_bits = 0;
    for(int i = 0; i < 4; ++i) {
        uint32_t x = io->get();
        if (!io->eof()) {
            m_bits = (m_bits << 8u) | (x & 0xffu);
            new_bits += 8;
        } else {
            io->clear();
            break;
        }
    }
    //io->read(reinterpret_cast<char *>(&t), 4);
    //std::cout << io->rdstate() << std::endl;
#if __BYTE_ORDER == __LITTLE_ENDIAN
    m_bits = bswap_32(m_bits);
#endif
    return new_bits;
}

uint8_t hl2demo::dstream::u1() {
    return byte();
}

int8_t hl2demo::dstream::s1() {
    return static_cast<int8_t>(ulong(8));
}

std::string hl2demo::dstream::read_until(char terminator) {
    std::string result;
    char c = byte();
    while (c != terminator) {
        result.push_back(c);
        c = byte();
    }
    return result;
}

bool hl2demo::dstream::bitbool() {
    return static_cast<bool>(ulong(1));
}

void hl2demo::dstream::init() {
    m_bits = 0;
    m_bits_available = 0;
    m_has_size = false;
    //io->exceptions(std::ios_base::badbit | std::ios_base::eofbit | std::ios_base::failbit);
}
