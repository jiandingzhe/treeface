/*
   A C-program for MT19937-64 (2014/2/23 version).
   Coded by Takuji Nishimura and Makoto Matsumoto.

   This is a 64-bit version of Mersenne Twister pseudorandom number
   generator.

   Before using, initialize the state by using init_genrand64(seed)
   or init_by_array64(init_key, key_length).

   Copyright (C) 2004, 2014, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

   References:
   T. Nishimura, ``Tables of 64-bit Mersenne Twisters''
     ACM Transactions on Modeling and
     Computer Simulation 10. (2000) 348--357.
   M. Matsumoto and T. Nishimura,
     ``Mersenne Twister: a 623-dimensionally equidistributed
       uniform pseudorandom number generator''
     ACM Transactions on Modeling and
     Computer Simulation 8. (Jan. 1998) 3--30.

   Any feedback is very welcome.
   http://www.math.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove spaces)
*/

#include "treeface/config.h"
#include "treejuce/File.h"
#include "treejuce/MT19937.h"
#include "treejuce/Time.h"

#include <stdio.h>
#include <inttypes.h>
#include <errno.h>

#if _MSC_VER
#  include <process.h>
#else
#  include <unistd.h>
#endif

using namespace std;

#define MM 156
#define MATRIX_A 0xB5026F5AA96619E9ull
#define UM 0xFFFFFFFF80000000ull /* Most significant 33 bits */
#define LM 0x7FFFFFFFull /* Least significant 31 bits */

TREEFACE_JUCE_NAMESPACE_BEGIN

MT19937 MT19937::easy;

uint64_t _get_seed_from_time_()
{
    return Time::getCurrentTime().toMilliseconds();
}

uint64_t _get_seed_from_pid_()
{
#ifdef TREEFACE_OS_WINDOWS
    uint32_t pid = uint32_t(_getpid());
    uint32_t ppid = uint32_t(_getpid());
#else
    uint32_t pid = uint32_t(getpid());
    uint32_t ppid = uint32_t(getppid());
#endif
    uint64_t result = pid;
    result <<= 32;
    result += ppid;
    return result;
}

#define RAND_DEV "/dev/urandom"

void _get_seed_from_dev_random_(uint64_t* seeds, size_t len)
{
    FILE* fh = fopen(RAND_DEV, "rb");
    if (!fh)
    {
        fprintf(stderr, "ERROR: MT19937 failed to open %s: %s\n",
                RAND_DEV, strerror(errno));
        abort();
    }


    if (fread(seeds, sizeof(uint64_t), len, fh) != len)
    {
        fprintf(stderr, "ERROR: MT19937 failed to read %s\n", RAND_DEV);
        abort();
    }
}

#if defined(_MSC_VER) && _MSC_VER >= 1400
void _get_seed_from_rand_s_(uint64_t* seeds, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        unsigned int rand_val = -1;
        rand_s(&rand_val);
        seeds[i] = rand_val;
    }
}
#endif

MT19937::MT19937()
{
    static bool has_dev_random = true;
    if (has_dev_random)
    {
        File path_dev_random("/dev/random");
        if (path_dev_random.existsAsFile())
        {
            uint64_t seeds[4];
            _get_seed_from_dev_random_(seeds, 4);
            set_seed_array(seeds, 4);
        }
        else
        {
            has_dev_random = false;
        }
    }

    if (!has_dev_random)
    {
#if defined(_MSC_VER) && _MSC_VER >= 1400
        uint64_t seeds[4];
        _get_seed_from_rand_s_(seeds, 4);
        set_seed_array(seeds, 4);
#else
        uint64_t seeds[2];
        seeds[0] = _get_seed_from_time_();
        seeds[1] = _get_seed_from_pid_();
        set_seed_array(seeds, 2);
#endif
    }
}

MT19937::MT19937(uint64_t seed)
{
    set_seed(seed);
}

MT19937::MT19937(uint64_t *seed_array, size_t len)
{
    set_seed_array(seed_array, len);
}

void MT19937::set_seed(uint64_t seed)
{
    mt[0] = seed;
    for (mti=1; mti<NN; mti++)
        mt[mti] =  (6364136223846793005ull * (mt[mti-1] ^ (mt[mti-1] >> 62)) + mti);
}

void MT19937::set_seed_array(uint64_t *seed_array, size_t len)
{
    unsigned int i, j;
    uint64_t k;
    set_seed(19650218ull);
    i=1; j=0;
    k = (NN > len ? NN : len);
    for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 62)) * 3935559000370003845ull))
          + seed_array[j] + j; /* non linear */
        i++; j++;
        if (i>=NN) { mt[0] = mt[NN-1]; i=1; }
        if (j>=len) j=0;
    }
    for (k=NN-1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 62)) * 2862933555777941757ull))
          - i; /* non linear */
        i++;
        if (i>=NN) { mt[0] = mt[NN-1]; i=1; }
    }

    mt[0] = 1ull << 63; /* MSB is 1; assuring non-zero initial array */
}

bool MT19937::next_bool()
{
    return next_uint64() > 0x7fffffffffffffffuLL;
}

uint64_t MT19937::next_uint64()
{
    int i;
    uint64_t x;
    static uint64_t mag01[2]={0ull, MATRIX_A};

    if (mti >= NN) { /* generate NN words at one time */

        /* if init_genrand64() has not been called, */
        /* a default initial seed is used     */
        if (mti == NN+1)
            set_seed(5489ull);

        for (i=0;i<NN-MM;i++) {
            x = (mt[i]&UM)|(mt[i+1]&LM);
            mt[i] = mt[i+MM] ^ (x>>1) ^ mag01[int(x & 1ull)];
        }
        for (;i<NN-1;i++) {
            x = (mt[i]&UM)|(mt[i+1]&LM);
            mt[i] = mt[i+(MM-NN)] ^ (x>>1) ^ mag01[int(x & 1ull)];
        }
        x = (mt[NN-1]&UM)|(mt[0]&LM);
        mt[NN-1] = mt[MM-1] ^ (x>>1) ^ mag01[int(x & 1ull)];

        mti = 0;
    }

    x = mt[mti++];

    x ^= (x >> 29) & 0x5555555555555555ull;
    x ^= (x << 17) & 0x71D67FFFEDA60000ull;
    x ^= (x << 37) & 0xFFF7EEE000000000ull;
    x ^= (x >> 43);

    return x;
}

uint64_t MT19937::next_uint64_in_range(uint64_t upper)
{
    uint64_t max64 = std::numeric_limits<uint64_t>::max();
    uint64_t raw_upper = max64 - max64 % upper;

    for (;;)
    {
        uint64_t re = next_uint64();
        if (re < raw_upper)
        {
            return re % upper;
        }
    }
}

int64_t MT19937::next_int63()
{
    return (int64_t)(next_uint64() >> 1);
}

double MT19937::next_double_yy()
{
    return (next_uint64() >> 11) * (1.0/9007199254740991.0);
}

double MT19937::next_double_yn()
{
    return (next_uint64() >> 11) * (1.0/9007199254740992.0);
}

double MT19937::next_double_nn()
{
    return ((next_uint64() >> 12) + 0.5) * (1.0/4503599627370496.0);
}

TREEFACE_JUCE_NAMESPACE_END
