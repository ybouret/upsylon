#include "y/os/endian.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{

    template <typename T>
    static inline bool test_endian()
    {
        for(size_t iter=0;iter<1024*1024;++iter)
        {
            const T x = alea.full<T>();
            {
                const T y = swap_le_as(x);
                const T z = swap_le_as(y);
                if(z!=x)
                {
                    std::cerr << "LE: bytes=" << sizeof(T) << ": " << uint64_t(x) << " -> " << uint64_t(y) << " -> " << uint64_t(z) << std::endl;
                    return false;
                }
            }

            {
                const T y = swap_be_as(x);
                const T z = swap_be_as(y);
                if(z!=x)
                {
                    std::cerr << "BE: bytes=" << sizeof(T) << ": " << uint64_t(x) << " -> " << uint64_t(y) << " -> " << uint64_t(z) << std::endl;
                    return false;
                }
            }
        }
        return true;
    }

    template <typename T>
    static inline bool test_big_endian()
    {
        std::cerr << std::hex;
        for(size_t iter=0;iter<1024*1024;++iter)
        {
            const T x = alea.full<T>();
            const T y = swap_be_as(x);
            const uint8_t *p = (const uint8_t *)&y;
            T z = 0;
            for(size_t i=0;i<sizeof(T);++i)
            {
                z <<= 8;
                z |= p[i];
            }
            if(z!=x)
            {
                std::cerr << "BigEndian: " << x << " => " << z << std::endl;
                return false;
            }
        }

        return true;
    }

}

Y_UTEST(endian)
{
    Y_CHECK(test_endian<uint8_t>());
    Y_CHECK(test_endian<uint16_t>());
    Y_CHECK(test_endian<uint32_t>());
    Y_CHECK(test_endian<uint64_t>());
    Y_CHECK(test_endian<int>());
    Y_CHECK(test_endian<short>());

    Y_CHECK(test_big_endian<uint16_t>());
    Y_CHECK(test_big_endian<uint32_t>());
    Y_CHECK(test_big_endian<uint64_t>());

}
Y_UTEST_DONE()

