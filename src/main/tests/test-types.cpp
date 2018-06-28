#include "y/type/ints.hpp"

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
	//select_type<true,double,float>::result x=0;
    test_select<double,int>();
    test_ints<short>();
    test_ints<unsigned short>();
    test_ints<int>();
    test_ints<unsigned int>();
    test_ints<long>();
    test_ints<unsigned long>();
}
Y_UTEST_DONE()
