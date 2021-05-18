//
// Created by umar on 10/12/20.
//


#ifndef DATA_TYPES_DIGEST_H
#define DATA_TYPES_DIGEST_H

#include "../base16.h"
#include <exception>


namespace Buffer { class Binary; }
namespace CPPBinary {
    class Digest {
        Buffer::Binary &m_buffer;

        Buffer::Binary result(const std::string &bytes);

    public:
        Digest();

        Digest(Buffer::Binary &mBuffer);

        Buffer::Binary digest(const std::string &algo, const int iterations = 1, const int bytes = 0);

        Buffer::Binary md5(int bytes = 0);

        Buffer::Binary sha1(int bytes = 0);

        Buffer::Binary sha256(int bytes = 0);

        Buffer::Binary ripeMd160(int bytes = 0);

        Buffer::Binary pbkdf2(const std::string &algo, Buffer::Binary salt, int iterations, int length=0);

        Buffer::Binary hmac(const std::string &algo, Buffer::Base16 key);

        Buffer::Binary hmac(const std::string &algo, Buffer::Binary key);

    };
};        // namespace CPPBinary
#endif    // DATA_TYPES_DIGEST_H
