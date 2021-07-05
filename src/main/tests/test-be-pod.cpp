#include "y/associative/be-pod.hpp"
#include "y/utest/run.hpp"
#include "y/type/rtti.hpp"

using namespace upsylon;

namespace
{
    template <typename T, size_t N>
    static inline void do_be_pod()
    {
        typedef BEpod<T,N> Type;
        std::cerr << "<" << rtti::name_of<T>() << "," << N << ">";
        std::cerr << " | size = " << std::setw(4) << Type::size;
        std::cerr << " | sz64 = " << std::setw(4) << Type::sz64;
        std::cerr << " | sizeof = " << std::setw(4) << sizeof(Type);
        std::cerr << std::endl;
    }

    template <typename T>
    static inline void do_be_pods()
    {
        do_be_pod<T,1>();
        do_be_pod<T,2>();
        do_be_pod<T,3>();
        do_be_pod<T,4>();
        do_be_pod<T,5>();
        do_be_pod<T,6>();
        do_be_pod<T,7>();
        do_be_pod<T,8>();
        std::cerr << std::endl;
    }

}

Y_UTEST(be_pod)
{
    do_be_pods<char>();
    do_be_pods<int16_t>();
    do_be_pods<int32_t>();
    do_be_pods<int64_t>();

}
Y_UTEST_DONE()

