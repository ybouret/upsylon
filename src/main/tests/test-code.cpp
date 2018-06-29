#include "y/code/round.hpp"
#include "y/hashing/hash64.hpp"
#include "y/utest/run.hpp"

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
            const T t1 = hashing::hash64::mix(t0,hashing::hash64::BJ);
            const T t2 = hashing::hash64::mix(t0,hashing::hash64::NR);
            std::cerr << uint64_t(t0) << ' ' << uint64_t(t1) << ' ' << uint64_t(t2) << std::endl;
        }
        std::cerr << std::dec;
    }
}

Y_UTEST(code)
{
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

    do_hash64<uint8_t>();
    do_hash64<uint16_t>();
    do_hash64<uint32_t>();
    do_hash64<uint64_t>();


}
Y_UTEST_DONE()


