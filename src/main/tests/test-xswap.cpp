#include "y/type/xswap.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include <cstring>

using namespace upsylon;

namespace {

    template <typename T, size_t N>
    static inline void doSwap()
    {
        T a[N];
        T b[N];
        T c[N];

        const size_t bytes = N * sizeof(T);

        for(size_t i=0;i<N;++i) a[i] = support::get<T>();
        memcpy(c,a,bytes);
        memset(b,0,bytes);

        for(size_t i=0;i<N;++i)
        {
            Y_ASSERT( a[i] == c[i]);
            Y_ASSERT( 0    == b[i] );
            xswap( a[i], b[i] );
            Y_ASSERT( b[i] == c[i] );
            Y_ASSERT( 0    == a[i] );
        }

    }

    template <typename T>
    void doBlock()
    {
        std::cerr << "doBlock<" << sizeof(T) << ">" << std::endl;
        std::cerr << "|_primitive=" << type_traits<T>::is_primitive << std::endl;
        doSwap<T,1>();
        doSwap<T,2>();
        doSwap<T,3>();
        doSwap<T,4>();
        doSwap<T,5>();
        doSwap<T,6>();
        doSwap<T,7>();
        doSwap<T,8>();
    }

}



Y_UTEST(xswap)
{
    doBlock<int8_t>();
    doBlock<int16_t>();
    doBlock<int32_t>();
    doBlock<int64_t>();
    doBlock<float>();
    doBlock<double>();
}
Y_UTEST_DONE()
