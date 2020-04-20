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

    std::cerr << std::hex;
    {
        const uint16_t x = 0x1234;
        const uint16_t y = swap_be(x);
        const uint16_t z = swap_be(y);
        std::cerr << x << "->" << y << "->" << z << std::endl;
    }
    
    {
        const uint32_t x = 0x12345678;
        const uint32_t y = swap_be(x);
        const uint32_t z = swap_be(y);
        std::cerr << x << "->" << y << "->" << z << std::endl;
    }
    {
        const uint64_t x = Y_U64(0x123456789abcef21);
        const uint64_t y = swap_be(x);
        const uint64_t z = swap_be(y);
        std::cerr << x << "->" << y << "->" << z << std::endl;
    }
    
}
Y_UTEST_DONE()

