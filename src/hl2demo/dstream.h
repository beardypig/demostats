#ifndef DEMOSTATS_DSTREAM_H
#define DEMOSTATS_DSTREAM_H

#include <istream>
#include <sstream>

#if defined(__APPLE__)
#include <machine/endian.h>
#include <libkern/OSByteOrder.h>
#define bswap_16(x) OSSwapInt16(x)
#define bswap_32(x) OSSwapInt32(x)
#define bswap_64(x) OSSwapInt64(x)
#define __BYTE_ORDER    BYTE_ORDER
#define __BIG_ENDIAN    BIG_ENDIAN
#define __LITTLE_ENDIAN LITTLE_ENDIAN
#elif defined(_MSC_VER) // !__APPLE__
#include <stdlib.h>
#define __LITTLE_ENDIAN     1234
#define __BIG_ENDIAN        4321
#define __BYTE_ORDER        __LITTLE_ENDIAN
#define bswap_16(x) _byteswap_ushort(x)
#define bswap_32(x) _byteswap_ulong(x)
#define bswap_64(x) _byteswap_uint64(x)
#else // !__APPLE__ or !_MSC_VER
#include <endian.h>
#include <byteswap.h>
#endif

namespace hl2demo {
    class dstream {
    private:
        void init();

        std::istream *io;
        std::istringstream io_str;

        static const uint32_t s_nMaskTable[33];
        size_t m_size;
        bool m_has_size;
        char read_buffer[2048];
    public:
        uint32_t m_bits_available;
        uint32_t m_bits;

        explicit dstream(std::istream *io);
        explicit dstream(const std::string &data);

        // stream positioning
        bool is_eof();

        void seekg(uint64_t pos);

        void seekg(uint64_t pos, std::ios_base::seekdir dir);

        uint64_t tellg();

        uint64_t size();

        // bit reading
        uint64_t ulong(uint32_t nbits);

        void read(char *buf, size_t len);

        std::string read(size_t len);

        std::string read_until(char terminator);

        bool bitbool();

        int32_t next_dword();

        uint8_t byte();

        uint8_t u1();

        int8_t s1();

        int16_t s2be();

        int32_t s4be();

        int64_t s8be();

        int16_t s2le();

        int32_t s4le();

        int64_t s8le();

        uint16_t u2be();

        uint32_t u4be();

        uint64_t u8be();

        uint16_t u2le();

        uint32_t u4le();

        uint64_t u8le();

        float f4le();

        float f4be();

        double f8le();

        double f8be();

        template<typename T>
        T vlq() {
            // VLQ encoding of integer
            uint8_t b;
            T result = 0;
            for (int i = 0; i < sizeof(T)+1; i++) {
                b = u1();
                result |= (b & 0x7fu) << (7u * i);
                if ((b & 0x80u) == 0) return result;
            }
            return result;
        }

        bool compare_fixed_string(const char* expected, size_t length);

        static std::string bytes_terminate(std::string src, char terminator = '\x00', bool include = false);

    };
}

#endif //DEMOSTATS_DSTREAM_H
