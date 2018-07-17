#include "y/code/round.hpp"
#include "y/hashing/hash64.hpp"
#include "y/utest/run.hpp"
#include "y/code/utils.hpp"
#include <cstdio>

using namespace upsylon;

namespace  {


    template <typename T>
    static inline void do_hash64()
    {
        std::cerr << std::dec;
        std::cerr << "hash64 for " << sizeof(T) << " bytes" << std::endl;
        std::cerr << std::hex;
        for(size_t iter=0;iter<10;++iter)
        {
            const T t0 = alea.full<T>();
            const T t1 = hashing::hash64::mix(t0,hashing::hash64::IBJ);
            const T t2 = hashing::hash64::mix(t0,hashing::hash64::DES);
            std::cerr << uint64_t(t0) << ' ' << uint64_t(t1) << ' ' << uint64_t(t2) << std::endl;
        }
        std::cerr << std::dec;
    }
}

Y_UTEST(code)
{
    std::cerr << "-- rounding" << std::endl;
    for(size_t i=0;i<=100;i+=3)
    {
        std::cerr << i
        << ' ' << Y_ROUND1(i)
        << ' ' << Y_ROUND2(i)
        << ' ' << Y_ROUND4(i)
        << ' ' << Y_ROUND8(i)
        << ' ' << Y_ROUND16(i)
        << ' ' << Y_ROUND32(i)
        << ' ' << Y_ROUND64(i)
        << ' ' << Y_ROUND128(i)
        << ' ' << Y_ROUND256(i)
        << std::endl;
    }

    std::cerr << "-- hash64" << std::endl;
    do_hash64<uint8_t>();
    do_hash64<uint16_t>();
    do_hash64<uint32_t>();
    do_hash64<uint64_t>();

    uint32_t lw = 1;
    uint32_t rw = 1;
    hashing::hash64::DES(&lw,&rw);
    Y_CHECK(lw==0x604D1DCE);
    Y_CHECK(rw==0x509C0C23);

    lw=1;rw=99;
    hashing::hash64::DES(&lw,&rw);
    Y_CHECK(lw==0xD97F8571);
    Y_CHECK(rw==0xA66CB41A);

    lw=99;rw=1;
    hashing::hash64::DES(&lw,&rw);
    Y_CHECK(lw==0x7822309D);
    Y_CHECK(rw==0x64300984);

    lw=99;rw=99;
    hashing::hash64::DES(&lw,&rw);
    Y_CHECK(lw==0xD7F376F0);
    Y_CHECK(rw==0x59BA89EB);

    std::cerr << "-- gen hexa" << std::endl;
    std::cerr.flush();
    for(unsigned i=0;i<256;++i)
    {
        fprintf(stderr," \"%02x\",",i);
    }
    fprintf(stderr,"\n");
    for(unsigned i=0;i<256;++i)
    {
        fprintf(stderr," \"%02X\",",i);
    }
    fprintf(stderr,"\n");
    fflush(stderr);

    std::cerr << "-- gen bits" << std::endl;
    for(size_t i=0;i<256;++i)
    {
        std::cerr << bits_for(i);
        if(i<255) std::cerr << ',';
        if( !((i+1)&15) ) std::cerr << std::endl;
    }
    std::cerr << std::endl;

    std::cerr << "isqrt" << std::endl;
    for(uint64_t i=1;i<limit_of<uint32_t>::maximum ; i += 1+alea.leq(1024) )
    {

        const uint32_t s = isqrt(i);
        //std::cerr << i << " -> " << s << std::endl;
        Y_ASSERT(s*s<=i);
    }

}
Y_UTEST_DONE()


