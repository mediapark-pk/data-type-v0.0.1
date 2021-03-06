/**
 * Copyright (c) 2000-2001 Aaron D. Gifford
 * Copyright (c) 2013-2014 Pavol Rusnak

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTOR(S) ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTOR(S) BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include "sha256.h"

#include <string>
#include <cstring>

static constexpr size_t SHA256_SHORT_BLOCK_LENGTH = SHA256_BLOCK_LENGTH - 8;

/* Hash constant words K for SHA-256: */
static constexpr std::array<uint32_t, 64> K256 = {
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL, 0x3956c25bUL, 0x59f111f1UL,
    0x923f82a4UL, 0xab1c5ed5UL, 0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
    0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL, 0xe49b69c1UL, 0xefbe4786UL,
    0x0fc19dc6UL, 0x240ca1ccUL, 0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
    0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL, 0xc6e00bf3UL, 0xd5a79147UL,
    0x06ca6351UL, 0x14292967UL, 0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
    0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL, 0xa2bfe8a1UL, 0xa81a664bUL,
    0xc24b8b70UL, 0xc76c51a3UL, 0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
    0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL, 0x391c0cb3UL, 0x4ed8aa4aUL,
    0x5b9cca4fUL, 0x682e6ff3UL, 0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
    0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL,
};

#if BYTE_ORDER == LITTLE_ENDIAN
static constexpr inline uint32_t Reverse32(uint32_t w)
{
    w = (w >> 16) | (w << 16);
    return (w & 0xff00ff00UL) >> 8 | (w & 0x00ff00ffUL) << 8;
}
#endif

// clang-format off
uint32_t inline Ch(uint32_t x, uint32_t y, uint32_t z) { return z ^ (x & (y ^ z)); }
uint32_t inline Maj(uint32_t x, uint32_t y, uint32_t z) { return (x & y) | (z & (x | y)); }
uint32_t inline Sigma0(uint32_t x) { return (x >> 2 | x << 30) ^ (x >> 13 | x << 19) ^ (x >> 22 | x << 10); }
uint32_t inline Sigma1(uint32_t x) { return (x >> 6 | x << 26) ^ (x >> 11 | x << 21) ^ (x >> 25 | x << 7); }
uint32_t inline sigma0(uint32_t x) { return (x >> 7 | x << 25) ^ (x >> 18 | x << 14) ^ (x >> 3); }
uint32_t inline sigma1(uint32_t x) { return (x >> 17 | x << 15) ^ (x >> 19 | x << 13) ^ (x >> 10); }
// clang-format on


void sha256_Init(SHA256_CTX* context)
{
    if (context == nullptr) {
        return;
    }
    context->state[0] = 0x6a09e667ul;
    context->state[1] = 0xbb67ae85ul;
    context->state[2] = 0x3c6ef372ul;
    context->state[3] = 0xa54ff53aul;
    context->state[4] = 0x510e527ful;
    context->state[5] = 0x9b05688cul;
    context->state[6] = 0x1f83d9abul;
    context->state[7] = 0x5be0cd19ul;
}

#define ROUND256_0_TO_15(a, b, c, d, e, f, g, h)                                                   \
    T1 = (h) + Sigma1(e) + Ch((e), (f), (g)) + K256[j] + W256[j];                                  \
    (d) += T1;                                                                                     \
    (h) = T1 + Sigma0(a) + Maj((a), (b), (c));                                                     \
    j++

#define ROUND256(a, b, c, d, e, f, g, h)                                                           \
    s0 = W256[(j + 1) & 0x0f];                                                                     \
    s0 = sigma0(s0);                                                                               \
    s1 = W256[(j + 14) & 0x0f];                                                                    \
    s1 = sigma1(s1);                                                                               \
    T1 = (h) + Sigma1(e) + Ch((e), (f), (g)) + K256[j] +                                           \
        (W256[j & 0x0f] += s1 + W256[(j + 9) & 0x0f] + s0);                                        \
    (d) += T1;                                                                                     \
    (h) = T1 + Sigma0(a) + Maj((a), (b), (c));                                                     \
    j++

void sha256_Transform(
    const std::array<uint32_t, 8>& state_in,
    const std::array<uint32_t, SHA256_BLOCK_LENGTH / sizeof(uint32_t)>& data,
    uint32_t* state_out)
// void sha256_Transform(const uint32_t* state_in, const uint32_t* data, uint32_t* state_out)
{
    /* Initialize registers with the prev. intermediate value */
    uint32_t a = state_in[0];
    uint32_t b = state_in[1];
    uint32_t c = state_in[2];
    uint32_t d = state_in[3];
    uint32_t e = state_in[4];
    uint32_t f = state_in[5];
    uint32_t g = state_in[6];
    uint32_t h = state_in[7];
    //    std::array<uint32_t, 8> x = state_in;
    //#define a (x[0])
    //#define b (x[1])
    //#define c (x[2])
    //#define d (x[3])
    //#define e (x[4])
    //#define f (x[5])
    //#define g (x[6])
    //#define h (x[7])

    uint32_t T1{}, T2{};

    std::array<uint32_t, 16> W256;
    std::copy(data.begin(), data.end(), W256.begin());

    int j = 0;
    ROUND256_0_TO_15(a, b, c, d, e, f, g, h);
    ROUND256_0_TO_15(h, a, b, c, d, e, f, g);
    ROUND256_0_TO_15(g, h, a, b, c, d, e, f);
    ROUND256_0_TO_15(f, g, h, a, b, c, d, e);
    ROUND256_0_TO_15(e, f, g, h, a, b, c, d);
    ROUND256_0_TO_15(d, e, f, g, h, a, b, c);
    ROUND256_0_TO_15(c, d, e, f, g, h, a, b);
    ROUND256_0_TO_15(b, c, d, e, f, g, h, a);
    ROUND256_0_TO_15(a, b, c, d, e, f, g, h);
    ROUND256_0_TO_15(h, a, b, c, d, e, f, g);
    ROUND256_0_TO_15(g, h, a, b, c, d, e, f);
    ROUND256_0_TO_15(f, g, h, a, b, c, d, e);
    ROUND256_0_TO_15(e, f, g, h, a, b, c, d);
    ROUND256_0_TO_15(d, e, f, g, h, a, b, c);
    ROUND256_0_TO_15(c, d, e, f, g, h, a, b);
    ROUND256_0_TO_15(b, c, d, e, f, g, h, a);
    //    do {
    //        /* Apply the SHA-256 compression function to update a..h with copy */
    //        T1 = h + Sigma1(e) + Ch(e, f, g) + K256.at(j) + W256.at(j);
    //        T2 = Sigma0(a) + Maj(a, b, c);
    //        h = g;
    //        g = f;
    //        f = e;
    //        e = d + T1;
    //        d = c;
    //        c = b;
    //        b = a;
    //        a = T1 + T2;

    //        ++j;
    //    } while (j < 16);
    uint32_t s0, s1;

    ROUND256(a, b, c, d, e, f, g, h);
    ROUND256(h, a, b, c, d, e, f, g);
    ROUND256(g, h, a, b, c, d, e, f);
    ROUND256(f, g, h, a, b, c, d, e);
    ROUND256(e, f, g, h, a, b, c, d);
    ROUND256(d, e, f, g, h, a, b, c);
    ROUND256(c, d, e, f, g, h, a, b);
    ROUND256(b, c, d, e, f, g, h, a);
    ROUND256(a, b, c, d, e, f, g, h);
    ROUND256(h, a, b, c, d, e, f, g);
    ROUND256(g, h, a, b, c, d, e, f);
    ROUND256(f, g, h, a, b, c, d, e);
    ROUND256(e, f, g, h, a, b, c, d);
    ROUND256(d, e, f, g, h, a, b, c);
    ROUND256(c, d, e, f, g, h, a, b);
    ROUND256(b, c, d, e, f, g, h, a);
    ROUND256(a, b, c, d, e, f, g, h);
    ROUND256(h, a, b, c, d, e, f, g);
    ROUND256(g, h, a, b, c, d, e, f);
    ROUND256(f, g, h, a, b, c, d, e);
    ROUND256(e, f, g, h, a, b, c, d);
    ROUND256(d, e, f, g, h, a, b, c);
    ROUND256(c, d, e, f, g, h, a, b);
    ROUND256(b, c, d, e, f, g, h, a);
    ROUND256(a, b, c, d, e, f, g, h);
    ROUND256(h, a, b, c, d, e, f, g);
    ROUND256(g, h, a, b, c, d, e, f);
    ROUND256(f, g, h, a, b, c, d, e);
    ROUND256(e, f, g, h, a, b, c, d);
    ROUND256(d, e, f, g, h, a, b, c);
    ROUND256(c, d, e, f, g, h, a, b);
    ROUND256(b, c, d, e, f, g, h, a);
    ROUND256(a, b, c, d, e, f, g, h);
    ROUND256(h, a, b, c, d, e, f, g);
    ROUND256(g, h, a, b, c, d, e, f);
    ROUND256(f, g, h, a, b, c, d, e);
    ROUND256(e, f, g, h, a, b, c, d);
    ROUND256(d, e, f, g, h, a, b, c);
    ROUND256(c, d, e, f, g, h, a, b);
    ROUND256(b, c, d, e, f, g, h, a);
    ROUND256(a, b, c, d, e, f, g, h);
    ROUND256(h, a, b, c, d, e, f, g);
    ROUND256(g, h, a, b, c, d, e, f);
    ROUND256(f, g, h, a, b, c, d, e);
    ROUND256(e, f, g, h, a, b, c, d);
    ROUND256(d, e, f, g, h, a, b, c);
    ROUND256(c, d, e, f, g, h, a, b);
    ROUND256(b, c, d, e, f, g, h, a);

    //    do {
    //        /* Part of the message block expansion: */
    //        uint32_t s0 = W256.at((j + 1) & 0x0f);
    //        s0 = sigma0(s0);
    //        uint32_t s1 = W256.at((j + 14) & 0x0f);
    //        s1 = sigma1(s1);

    //        /* Apply the SHA-256 compression function to update a..h */
    //        T1 = h + Sigma1(e) + Ch(e, f, g) + K256.at(j) +
    //            (W256.at(j & 0x0f) += s1 + W256.at((j + 9) & 0x0f) + s0);
    //        T2 = Sigma0(a) + Maj(a, b, c);
    //        h = g;
    //        g = f;
    //        f = e;
    //        e = d + T1;
    //        d = c;
    //        c = b;
    //        b = a;
    //        a = T1 + T2;

    //        ++j;
    //    } while (j < 64);

    /* Compute the current intermediate hash value */
    state_out[0] = state_in[0] + a;
    state_out[1] = state_in[1] + b;
    state_out[2] = state_in[2] + c;
    state_out[3] = state_in[3] + d;
    state_out[4] = state_in[4] + e;
    state_out[5] = state_in[5] + f;
    state_out[6] = state_in[6] + g;
    state_out[7] = state_in[7] + h;
}

void sha256_Update(SHA256_CTX* context, const uint8_t* data, size_t len)
{
    if (len == 0) {
        /* Calling with no data is valid - we do nothing */
        return;
    }

    unsigned int usedspace = (context->bitcount >> 3) % SHA256_BLOCK_LENGTH;
    if (usedspace > 0) {
        /* Calculate how much free space is available in the buffer */
        unsigned int freespace = SHA256_BLOCK_LENGTH - usedspace;

        if (len >= freespace) {
            /* Fill the buffer completely and process it */
            std::memcpy(
                reinterpret_cast<uint8_t*>(context->buffer.data()) + usedspace, data, freespace);
            context->bitcount += freespace << 3;
            len -= freespace;
            data += freespace;
#if BYTE_ORDER == LITTLE_ENDIAN
            /* Convert TO host byte order */
            for (int j = 0; j < 16; ++j) {
                context->buffer[j] = Reverse32(context->buffer[j]);
            }
#endif
            sha256_Transform(context->state, context->buffer, context->state.data());
        } else {
            /* The buffer is not yet full */
            std::memcpy(reinterpret_cast<uint8_t*>(context->buffer.data()) + usedspace, data, len);
            context->bitcount += len << 3;
            /* Clean up: */
            usedspace = freespace = 0;
            return;
        }
    }
    while (len >= SHA256_BLOCK_LENGTH) {
        /* Process as many complete blocks as we can */
        std::memcpy(context->buffer.data(), data, SHA256_BLOCK_LENGTH);
#if BYTE_ORDER == LITTLE_ENDIAN
        /* Convert TO host byte order */
        for (int j = 0; j < 16; ++j) {
            context->buffer[j] = Reverse32(context->buffer[j]);
        }
#endif
        sha256_Transform(context->state, context->buffer, context->state.data());
        context->bitcount += SHA256_BLOCK_LENGTH << 3;
        len -= SHA256_BLOCK_LENGTH;
        data += SHA256_BLOCK_LENGTH;
    }
    if (len > 0) {
        /* There's left-overs, so save 'em */
        std::memcpy(context->buffer.data(), data, len);
        context->bitcount += len << 3;
    }
}

void sha256_Final(SHA256_CTX* context, uint8_t digest[])
{
    /* If no digest buffer is passed, we don't bother doing this: */
    if (digest != nullptr) {
        unsigned int usedspace = (context->bitcount >> 3) % SHA256_BLOCK_LENGTH;
        /* Begin padding with a 1 bit: */
        reinterpret_cast<uint8_t*>(context->buffer.data())[usedspace++] = 0x80;

        if (usedspace > SHA256_SHORT_BLOCK_LENGTH) {
            std::fill_n(
                reinterpret_cast<uint8_t*>(context->buffer.data()) + usedspace,
                SHA256_BLOCK_LENGTH - usedspace,
                0);

#if BYTE_ORDER == LITTLE_ENDIAN
            /* Convert TO host byte order */
            //            for (int j = 0; j < 16; ++j) {
            context->buffer[0] = Reverse32(context->buffer[0]);
            context->buffer[1] = Reverse32(context->buffer[1]);
            context->buffer[2] = Reverse32(context->buffer[2]);
            context->buffer[3] = Reverse32(context->buffer[3]);
            context->buffer[4] = Reverse32(context->buffer[4]);
            context->buffer[5] = Reverse32(context->buffer[5]);
            context->buffer[6] = Reverse32(context->buffer[6]);
            context->buffer[7] = Reverse32(context->buffer[7]);
            context->buffer[8] = Reverse32(context->buffer[8]);
            context->buffer[9] = Reverse32(context->buffer[9]);
            context->buffer[10] = Reverse32(context->buffer[10]);
            context->buffer[11] = Reverse32(context->buffer[11]);
            context->buffer[12] = Reverse32(context->buffer[12]);
            context->buffer[13] = Reverse32(context->buffer[13]);
            context->buffer[14] = Reverse32(context->buffer[14]);
            context->buffer[15] = Reverse32(context->buffer[15]);

//                context->buffer[j] = Reverse32(context->buffer[j]);
//            }
#endif
            /* Do second-to-last transform: */
            sha256_Transform(context->state, context->buffer, context->state.data());

            /* And prepare the last transform: */
            usedspace = 0;
        }
        /* Set-up for the last transform: */
        std::fill_n(
            reinterpret_cast<uint8_t*>(context->buffer.data()) + usedspace,
            SHA256_SHORT_BLOCK_LENGTH - usedspace,
            0);

#if BYTE_ORDER == LITTLE_ENDIAN
        /* Convert TO host byte order */
        //        for (int j = 0; j < 14; ++j) {
        context->buffer[0] = Reverse32(context->buffer[0]);
        context->buffer[1] = Reverse32(context->buffer[1]);
        context->buffer[2] = Reverse32(context->buffer[2]);
        context->buffer[3] = Reverse32(context->buffer[3]);
        context->buffer[4] = Reverse32(context->buffer[4]);
        context->buffer[5] = Reverse32(context->buffer[5]);
        context->buffer[6] = Reverse32(context->buffer[6]);
        context->buffer[7] = Reverse32(context->buffer[7]);
        context->buffer[8] = Reverse32(context->buffer[8]);
        context->buffer[9] = Reverse32(context->buffer[9]);
        context->buffer[10] = Reverse32(context->buffer[10]);
        context->buffer[11] = Reverse32(context->buffer[11]);
        context->buffer[12] = Reverse32(context->buffer[12]);
        context->buffer[13] = Reverse32(context->buffer[13]);
//        }
#endif
        /* Set the bit count: */
        context->buffer[14] = context->bitcount >> 32;
        context->buffer[15] = context->bitcount & 0xffffffff;

        /* Final transform: */
        sha256_Transform(context->state, context->buffer, context->state.data());

#if BYTE_ORDER == LITTLE_ENDIAN
        /* Convert FROM host byte order */
        //        for (int j = 0; j < 8; ++j) {
        context->state[0] = Reverse32(context->state[0]);
        context->state[1] = Reverse32(context->state[1]);
        context->state[2] = Reverse32(context->state[2]);
        context->state[3] = Reverse32(context->state[3]);
        context->state[4] = Reverse32(context->state[4]);
        context->state[5] = Reverse32(context->state[5]);
        context->state[6] = Reverse32(context->state[6]);
        context->state[7] = Reverse32(context->state[7]);
//        }
#endif
        std::memcpy(digest, context->state.data(), SHA256_RAW_BYTES_LENGTH);
    }

    /* Clean up state data: */
    std::memset(context, 0, sizeof(SHA256_CTX));
}

void sha256(const uint8_t* data, size_t len, uint8_t digest[SHA256_RAW_BYTES_LENGTH])
{
    SHA256_CTX context;
    sha256_Init(&context);
    sha256_Update(&context, data, len);
    sha256_Final(&context, digest);
}

template <>
std::vector<uint8_t> sha256<std::string>(const std::string& data)
{
    std::vector<uint8_t> output(SHA256_RAW_BYTES_LENGTH);
    sha256(reinterpret_cast<const uint8_t*>(data.c_str()), data.size(), &output[0]);
    return output;
}
