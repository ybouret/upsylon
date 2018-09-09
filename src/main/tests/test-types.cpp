#include "y/type/ints.hpp"
#include "y/type/fourcc.hpp"

#include "y/utest/run.hpp"
#include <typeinfo>

using namespace upsylon;

template <typename A,typename B>
static inline void test_select()
{
    const std::type_info &a = typeid(A);
    const std::type_info &b = typeid(B);
    typedef typename select_type<true, A,B>::result sa_type;
    typedef typename select_type<false,A,B>::result sb_type;
    Y_CHECK(typeid(sa_type)==a);
    Y_CHECK(typeid(sb_type)==b);

}

template <typename T>
static inline void test_ints()
{
    std::cerr << typeid(T).name() << std::endl;
    std::cerr << "\tmin=" << limit_of<T>::minimun << std::endl;
    std::cerr << "\tmax=" << limit_of<T>::maximum << std::endl;
}


Y_UTEST(types)
{
    test_select<double,int>();
    test_ints<short>();
    test_ints<unsigned short>();
    test_ints<int>();
    test_ints<unsigned int>();
    test_ints<long>();
    test_ints<unsigned long>();
    const uint32_t t1 = Y_FOURCC(1,2,3,4);
    const uint32_t t2 = Y_FOURCC(4,3,2,1);
    std::cerr << std::hex << t1 << " | " << t2 << std::endl;
}
Y_UTEST_DONE()
