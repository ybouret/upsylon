#include "y/type/select.hpp"
#include "y/type/limits.hpp"

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

Y_UTEST(types)
{
    test_select<double,int>();
}
Y_UTEST_DONE()
