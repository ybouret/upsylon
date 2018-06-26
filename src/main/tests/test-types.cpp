#include "y/type/ints.hpp"

#include "y/utest/run.hpp"
#include <typeinfo>

using namespace upsylon;

template <typename A,typename B>
static inline void test_select()
{
    const std::type_info &a = typeid(A);
    const std::type_info &b = typeid(B);
    Y_CHECK(typeid(typename select<true, A,B>::result)==a);
    Y_CHECK(typeid(typename select<false,A,B>::result)==b);

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
}
Y_UTEST_DONE()
