
#include "y/type/is-aligned.hpp"
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

#include <iomanip>
#include "y/string.hpp"
#include "y/type/spec.hpp"

namespace {

    template <typename T>
    void XALIGN()
    {
        std::cerr << "align for " << type_name_of<T>() << std::endl;
        typedef aligned_type_for<T> aligned_type;
        std::cerr << "\tkind      : " << aligned_type::text()    << std::endl;
        std::cerr << "\ttype_size : " << aligned_type::type_size << std::endl;
        std::cerr << "\tword_size : " << aligned_type::word_size << std::endl;
        std::cerr << "\twords     : " << aligned_type::words     << std::endl;

        T source = support::get<T>();
        const T source_org = source;
        T target = support::get<T>();
        const T target_org = target;

        bswap(target,source);
        Y_CHECK(target_org==source);
        Y_CHECK(source_org==target);


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

    for(size_t i=0;i<=100;++i)
    {
        is_aligned::value v = Y_IS_ALIGNED_VALUE(i);
        std::cerr << std::setw(4) << i << " -> " << is_aligned::text(v) << std::endl;
    }

    XALIGN<uint8_t>();
    XALIGN<uint16_t>();
    XALIGN<uint32_t>();
    XALIGN<uint64_t>();
    XALIGN<string>();
    
}
Y_UTEST_DONE()
