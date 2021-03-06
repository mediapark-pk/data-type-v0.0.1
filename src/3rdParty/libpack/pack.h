// (c) 2020 DDK
// This code is licensed under MIT license (see LICENSE for details)

#ifndef PACK_H
#define PACK_H

// (c) 2020 DDK
// This code is licensed under MIT license (see LICENSE for details)

#include <array>
#include <climits>
#include <cstdint>
#include <string.h>

/** Byteswap */
#ifdef _MSC_VER

#include <stdlib.h>
#define bswap_32(x) _byteswap_ulong(x)
#define bswap_64(x) _byteswap_uint64(x)

#elif defined(__APPLE__)

// Mac OS X / Darwin features
#include <libkern/OSByteOrder.h>
#define bswap_32(x) OSSwapInt32(x)
#define bswap_64(x) OSSwapInt64(x)

#elif defined(__sun) || defined(sun)

#include <sys/byteorder.h>
#define bswap_32(x) BSWAP_32(x)
#define bswap_64(x) BSWAP_64(x)

#elif defined(__FreeBSD__)

#include <sys/endian.h>
#define bswap_32(x) bswap32(x)
#define bswap_64(x) bswap64(x)

#elif defined(__OpenBSD__)

#include <sys/types.h>
#define bswap_32(x) swap32(x)
#define bswap_64(x) swap64(x)

#elif defined(__NetBSD__)

#include <machine/bswap.h>
#include <sys/types.h>
#if defined(__BSWAP_RENAME) && !defined(__bswap_32)
#define bswap_32(x) bswap32(x)
#define bswap_64(x) bswap64(x)
#endif

#else
    #if HAVE_BYTESWAP_H
        #include <byteswap.h>
    #else
        #define bswap_16(value) \
        ((((value) & 0xff) << 8) | ((value) >> 8))

        #define bswap_32(value) \
        (((uint32_t)bswap_16((uint16_t)((value) & 0xffff)) << 16) | \
        (uint32_t)bswap_16((uint16_t)((value) >> 16)))

        #define bswap_64(value) \
        (((uint64_t)bswap_32((uint32_t)((value) & 0xffffffff)) \
        << 32) | \
        (uint64_t)bswap_32((uint32_t)((value) >> 32)))
    #endif
#endif

namespace PhPacker
{
#if defined(__x86_64__) || defined(__LP64__) || defined(_LP64) || defined(_WIN64)
#define ENABLE_LONG64 1
#endif

#ifdef ENABLE_LONG64
#define SIZEOF_LONG 8
#else
#define SIZEOF_LONG 4
#endif

/* Whether machine is little endian */
static char machine_little_endian;

/* Mapping of byte from char (8bit) to long for machine endian */
static std::array<int, 1> byte_map;
// static int byte_map[1];

/* Mappings of bytes from int (machine dependant) to int for machine endian */
static std::array<int, sizeof(int)> int_map;
// static int int_map[sizeof(int)];

/* Mappings of bytes from shorts (16bit) for all endian environments */
static std::array<int, 2> machine_endian_short_map;
static std::array<int, 2> big_endian_short_map;
static std::array<int, 2> little_endian_short_map;

/* Mappings of bytes from longs (32bit) for all endian environments */
static std::array<int, 4> machine_endian_long_map;
static std::array<int, 4> big_endian_long_map;
static std::array<int, 4> little_endian_long_map;

#if SIZEOF_LONG > 4
/* Mappings of bytes from quads (64bit) for all endian environments */
static std::array<int, 8> machine_endian_longlong_map;
static std::array<int, 8> big_endian_longlong_map;
static std::array<int, 8> little_endian_longlong_map;
#endif

void init() noexcept
{
    static bool initialized = false;
    if (initialized)
        return;
    int machine_endian_check = 1;
    machine_little_endian = reinterpret_cast<char*>(&machine_endian_check)[0];

    if (machine_little_endian) {
        /* Where to get lo to hi bytes from */
        byte_map[0] = 0;

        for (int i = 0; i < static_cast<int>(sizeof(int)); ++i) {
            int_map[i] = i;
        }

        machine_endian_short_map[0] = 0;
        machine_endian_short_map[1] = 1;
        big_endian_short_map[0] = 1;
        big_endian_short_map[1] = 0;
        little_endian_short_map[0] = 0;
        little_endian_short_map[1] = 1;

        machine_endian_long_map[0] = 0;
        machine_endian_long_map[1] = 1;
        machine_endian_long_map[2] = 2;
        machine_endian_long_map[3] = 3;
        big_endian_long_map[0] = 3;
        big_endian_long_map[1] = 2;
        big_endian_long_map[2] = 1;
        big_endian_long_map[3] = 0;
        little_endian_long_map[0] = 0;
        little_endian_long_map[1] = 1;
        little_endian_long_map[2] = 2;
        little_endian_long_map[3] = 3;
#if SIZEOF_LONG > 4
        machine_endian_longlong_map[0] = 0;
        machine_endian_longlong_map[1] = 1;
        machine_endian_longlong_map[2] = 2;
        machine_endian_longlong_map[3] = 3;
        machine_endian_longlong_map[4] = 4;
        machine_endian_longlong_map[5] = 5;
        machine_endian_longlong_map[6] = 6;
        machine_endian_longlong_map[7] = 7;
        big_endian_longlong_map[0] = 7;
        big_endian_longlong_map[1] = 6;
        big_endian_longlong_map[2] = 5;
        big_endian_longlong_map[3] = 4;
        big_endian_longlong_map[4] = 3;
        big_endian_longlong_map[5] = 2;
        big_endian_longlong_map[6] = 1;
        big_endian_longlong_map[7] = 0;
        little_endian_longlong_map[0] = 0;
        little_endian_longlong_map[1] = 1;
        little_endian_longlong_map[2] = 2;
        little_endian_longlong_map[3] = 3;
        little_endian_longlong_map[4] = 4;
        little_endian_longlong_map[5] = 5;
        little_endian_longlong_map[6] = 6;
        little_endian_longlong_map[7] = 7;
#endif

    } else {
        int val;
        int size = sizeof((val));

        /* Where to get hi to lo bytes from */
        byte_map[0] = size - 1;

        for (int i = 0; i < (int)sizeof(int); ++i) {
            int_map[i] = size - (sizeof(int) - i);
        }

        machine_endian_short_map[0] = size - 2;
        machine_endian_short_map[1] = size - 1;
        big_endian_short_map[0] = size - 2;
        big_endian_short_map[1] = size - 1;
        little_endian_short_map[0] = size - 1;
        little_endian_short_map[1] = size - 2;

        machine_endian_long_map[0] = size - 4;
        machine_endian_long_map[1] = size - 3;
        machine_endian_long_map[2] = size - 2;
        machine_endian_long_map[3] = size - 1;
        big_endian_long_map[0] = size - 4;
        big_endian_long_map[1] = size - 3;
        big_endian_long_map[2] = size - 2;
        big_endian_long_map[3] = size - 1;
        little_endian_long_map[0] = size - 1;
        little_endian_long_map[1] = size - 2;
        little_endian_long_map[2] = size - 3;
        little_endian_long_map[3] = size - 4;
#if SIZEOF_LONG > 4
        machine_endian_longlong_map[0] = size - 8;
        machine_endian_longlong_map[1] = size - 7;
        machine_endian_longlong_map[2] = size - 6;
        machine_endian_longlong_map[3] = size - 5;
        machine_endian_longlong_map[4] = size - 4;
        machine_endian_longlong_map[5] = size - 3;
        machine_endian_longlong_map[6] = size - 2;
        machine_endian_longlong_map[7] = size - 1;
        big_endian_longlong_map[0] = size - 8;
        big_endian_longlong_map[1] = size - 7;
        big_endian_longlong_map[2] = size - 6;
        big_endian_longlong_map[3] = size - 5;
        big_endian_longlong_map[4] = size - 4;
        big_endian_longlong_map[5] = size - 3;
        big_endian_longlong_map[6] = size - 2;
        big_endian_longlong_map[7] = size - 1;
        little_endian_longlong_map[0] = size - 1;
        little_endian_longlong_map[1] = size - 2;
        little_endian_longlong_map[2] = size - 3;
        little_endian_longlong_map[3] = size - 4;
        little_endian_longlong_map[4] = size - 5;
        little_endian_longlong_map[5] = size - 6;
        little_endian_longlong_map[6] = size - 7;
        little_endian_longlong_map[7] = size - 8;
#endif
    }
    initialized = true;
}

template <typename T>
void php_pack(const T val, size_t size, int* map, std::string& output) noexcept
{
    const char* v = reinterpret_cast<const char*>(&val);

    output.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        output.push_back(v[map[i]]);
    }
}

inline double ToDouble(int value)
{
    return static_cast<double>(value);
}

float php_pack_parse_float(int is_little_endian, const char* src) noexcept
{
    char out[4] = {};
    if (!is_little_endian) {
        out[0] = src[3];
        out[1] = src[2];
        out[2] = src[1];
        out[3] = src[0];
    } else {
        out[0] = src[0];
        out[1] = src[1];
        out[2] = src[2];
        out[3] = src[3];
    }
    float f {};
    memcpy(&f, out, sizeof(float));
    return f;
}

double php_pack_parse_double(int is_little_endian, const char* src) noexcept
{
    char out[8] = {};
    if (!is_little_endian) {
        out[0] = src[7];
        out[1] = src[6];
        out[2] = src[5];
        out[3] = src[4];
        out[4] = src[3];
        out[5] = src[2];
        out[6] = src[1];
        out[7] = src[0];
    } else {
        out[0] = src[0];
        out[1] = src[1];
        out[2] = src[2];
        out[3] = src[3];
        out[4] = src[4];
        out[5] = src[5];
        out[6] = src[6];
        out[7] = src[7];
    }
    double d {};
    memcpy(&d, out, sizeof(double));

    return d;
}

void php_pack_copy_float(int is_little_endian, std::array<char, sizeof(float)>& dst, float f) noexcept
{
    char src[4] = {};
    memcpy(src, &f, sizeof(float));
    if (!is_little_endian) {
        dst[0] = src[3];
        dst[1] = src[2];
        dst[2] = src[1];
        dst[3] = src[0];
    } else {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
    }
}

void php_pack_copy_double(int is_little_endian, std::array<char, sizeof(double)>& dst, double d) noexcept
{
    char src[8] = {};
    memcpy(src, &d, sizeof(double));
    if (!is_little_endian) {
        dst[0] = src[7];
        dst[1] = src[6];
        dst[2] = src[5];
        dst[3] = src[4];
        dst[4] = src[3];
        dst[5] = src[2];
        dst[6] = src[1];
        dst[7] = src[0];
    } else {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        dst[4] = src[4];
        dst[5] = src[5];
        dst[6] = src[6];
        dst[7] = src[7];
    }
}

/**
 * @brief pack
 * @param code
 * @param val
 * @return string
 */
template <typename T,
    typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
std::string pack(char code, const T val) noexcept
{
    init();
    std::string output;

    switch (code) {
    case 'c':
    case 'C':
        php_pack(val, 1, byte_map.data(), output);
        break;
    case 's':
    case 'S':
    case 'n':
    case 'v': {
        auto map = machine_endian_short_map.data();
        auto v = static_cast<short>(val);

        if (code == 'n') {
            map = big_endian_short_map.data();
            v = static_cast<unsigned short>(val);
        } else if (code == 'v') {
            map = little_endian_short_map.data();
            v = static_cast<unsigned short>(val);
        } else if (code == 'S') {
            v = static_cast<unsigned short>(val);
        }

        php_pack(v, 2, map, output);
        break;
    }
    case 'i':
    case 'I':
        php_pack(val, sizeof(int), int_map.data(), output);
        break;
    case 'l':
    case 'L':
    case 'N':
    case 'V': {
        auto map = machine_endian_long_map;
        long v = static_cast<long long>(val);

        if (code == 'N') {
            map = big_endian_long_map;
            v = static_cast<unsigned long>(val);
        } else if (code == 'V') {
            map = little_endian_long_map;
            v = static_cast<unsigned long>(val);
        } else if (code == 'L') {
            v = static_cast<unsigned long>(val);
        }

        php_pack(v, 4, map.data(), output);
        break;
    }
    case 'q':
    case 'Q':
    case 'J':
    case 'P': {
        auto map = machine_endian_longlong_map;
        long long v = static_cast<long long>(val);

        if (code == 'J') {
            v = static_cast<unsigned long long>(val);
            map = big_endian_longlong_map;
        } else if (code == 'P') {
            v = static_cast<unsigned long long>(val);
            map = little_endian_longlong_map;
        }

        php_pack(v, 8, map.data(), output);
        break;
    }
    case 'f': {
        float v = static_cast<float>(val);
        std::array<char, sizeof(float)> out = {};
        memcpy(out.data(), &v, sizeof(v));
        output.assign(out.begin(), out.end());
        break;
    }
    case 'g': {
        /* pack little endian float */
        float v = static_cast<float>(val);
        std::array<char, sizeof(float)> out = {};
        php_pack_copy_float(1, out, v);
        output.assign(out.begin(), out.end());

        break;
    }
    case 'G': {
        /* pack big endian float */
        float v = static_cast<float>(val);
        std::array<char, sizeof(float)> out = {};
        php_pack_copy_float(0, out, v);
        output.assign(out.begin(), out.end());
        break;
    }
    case 'd': {
        double v = static_cast<double>(val);
        std::array<char, sizeof(double)> out = {};
        memcpy(out.data(), &v, sizeof(v));
        output.assign(out.begin(), out.end());
    } break;

    case 'e': {
        /* pack little endian double */
        double v = static_cast<double>(val);
        std::array<char, sizeof(double)> out = {};
        php_pack_copy_double(1, out, v);
        output.assign(out.begin(), out.end());
        break;
    }

    case 'E': {
        /* pack big endian double */
        double v = static_cast<double>(val);
        std::array<char, sizeof(double)> out = {};
        php_pack_copy_double(0, out, v);
        output.assign(out.begin(), out.end());
        break;
    }
    }
    return output;
}

template <typename T>
T php_unpack(const char* data, int size, bool issigned, int* map) noexcept
{
    long result {};
    char* cresult = reinterpret_cast<char*>(&result);

    result = issigned ? -1 : 0;

    for (int i = 0; i < size; ++i) {
        cresult[map[i]] = *data++;
    }

    return result;
}

/**
 * @brief unpack
 * @param format
 * @param data
 * @return long
 */
template <typename T>
T unpack(char format, const std::string& data) noexcept
{
    init();
    int size = 0;
    const int inputlen = data.length();

    switch (format) {
    /* Use 1 byte of input */
    case 'c':
    case 'C':
        size = 1;
        break;
    /* Never use any input */
    case 's':
    case 'S':
    case 'n':
    case 'v':
        size = 2;
        break;
    case 'i':
    case 'I':
        size = sizeof(int);
        break;
    /* Use 4 bytes of input */
    case 'l':
    case 'L':
    case 'N':
    case 'V':
        size = 4;
        break;
#if SIZEOF_LONG > 4
    case 'q':
    case 'Q':
    case 'J':
    case 'P':
        size = 8;
        break;
#endif
    /* Use sizeof(float) bytes of input */
    case 'f':
    case 'g':
    case 'G':
        size = sizeof(float);
        break;
    case 'd':
    case 'e':
    case 'E':
        size = sizeof(double);
        break;
    default:
        break;
    }

    /* Do actual unpacking */
    if ((size) <= inputlen) {
        switch (format) {
        case 'c':
        case 'C': {
            bool isSigned = (format == 'c') ? (data[0] & 0x80) : 0;
            if (format == 'c') {
                auto v = php_unpack<signed char>(data.c_str(), 1, isSigned, byte_map.data());
                return v;
            } else {
                auto v = php_unpack<unsigned char>(data.c_str(), 1, isSigned, byte_map.data());
                return v;
            }
            break;
        }
        case 's':
        case 'S':
        case 'n':
        case 'v': {
            bool isSigned = false;
            int* map = machine_endian_short_map.data();

            if (format == 's') {
                isSigned = data[(machine_little_endian ? 1 : 0)] & 0x80;
                /* return here for signed short */
                auto v = php_unpack<short>(data.c_str(), 2, isSigned, map);
                return v;
            } else if (format == 'n') {
                map = big_endian_short_map.data();
            } else if (format == 'v') {
                map = little_endian_short_map.data();
            }

            auto v = php_unpack<unsigned short>(data.c_str(), 2, isSigned, map);
            return v;
        }
        case 'i':
        case 'I': {
            bool isSigned = false;
            if (format == 'i') {
                isSigned = data[(machine_little_endian ? (sizeof(int) - 1) : 0)] & 0x80;
                int v = php_unpack<int>(data.c_str(), sizeof(int), isSigned, int_map.data());
                return v;
            } else {
                unsigned int v = php_unpack<unsigned int>(data.c_str(), sizeof(int), isSigned, int_map.data());
                return v;
            }
        }
        case 'l':
        case 'L':
        case 'N':
        case 'V': {
            bool issigned = false;
            int* map = machine_endian_long_map.data();
            long v {};

            if (format == 'l' || format == 'L') {
                issigned = data[machine_little_endian ? 3 : 0] & 0x80;
            } else if (format == 'N') {
                issigned = data[0] & 0x80;
                map = big_endian_long_map.data();
            } else if (format == 'V') {
                issigned = data[3] & 0x80;
                map = little_endian_long_map.data();
            }

            if (SIZEOF_LONG > 4 && issigned) {
                v = ~INT_MAX;
            }

            v |= php_unpack<long>(data.c_str(), 4, issigned, map);
            if (SIZEOF_LONG > 4) {
                if (format == 'l') {
                    v = (signed long)v;
                } else {
                    v = (unsigned long)v;
                }
            }
            return v;
        }

#if SIZEOF_LONG > 4
        case 'q':
        case 'Q':
        case 'J':
        case 'P': {
            bool isSigned = 0;
            auto map = machine_endian_longlong_map;

            if (format == 'q' || format == 'Q') {
                isSigned = data[machine_little_endian ? 7 : 0] & 0x80;
            } else if (format == 'J') {
                isSigned = data[0] & 0x80;
                map = big_endian_longlong_map;
            } else if (format == 'P') {
                isSigned = data[7] & 0x80;
                map = little_endian_longlong_map;
            }

            if (format == 'q') {
                auto v = php_unpack<int64_t>(data.data(), 8, isSigned, map.data());
                return v;
            } else {
                auto v = php_unpack<uint64_t>(data.data(), 8, isSigned, map.data());
                return v;
            }
        }
#endif
        case 'f': /* float */
        case 'g': /* little endian float*/
        case 'G': /* big endian float*/
        {
            float v {};

            if (format == 'g') {
                v = php_pack_parse_float(1, data.data());
            } else if (format == 'G') {
                v = php_pack_parse_float(0, data.data());
            } else {
                memcpy(&v, data.data(), sizeof(float));
            }
            return v;

            break;
        }

        case 'd': /* double */
        case 'e': /* little endian float */
        case 'E': /* big endian float */
        {
            double v {};
            if (format == 'e') {
                v = php_pack_parse_double(1, data.data());
            } else if (format == 'E') {
                v = php_pack_parse_double(0, data.data());
            } else {
                memcpy(&v, data.data(), sizeof(double));
            }
            return v;
            break;
        }
        }
    }
    return static_cast<T>(-1);
}

} // namespace PhPacker

#endif /* PACK_H */
