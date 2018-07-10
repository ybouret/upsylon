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
                const T y = swap_le(x);
                const T z = swap_le(y);
                if(z!=x)
                {
                    std::cerr << "LE: bytes=" << sizeof(T) << ": " << uint64_t(x) << " -> " << uint64_t(y) << " -> " << uint64_t(z) << std::endl;
                    return false;
                }
            }

            {
                const T y = swap_be(x);
                const T z = swap_be(y);
                if(z!=x)
                {
                    std::cerr << "BE: bytes=" << sizeof(T) << ": " << uint64_t(x) << " -> " << uint64_t(y) << " -> " << uint64_t(z) << std::endl;
                    return false;
                }
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

}
Y_UTEST_DONE()

