
#include "y/container/addresses.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    static inline void check_addr( addresses<T> &addr )
    {
        const addresses<T> &cst = addr;
        for(size_t i=0;i<addr.size;++i)
        {
            Y_ASSERT(cst[i]==addr[i]);
            Y_ASSERT(T(i)==addr[i]);
        }
    }
}

Y_UTEST(addresses)
{
    Y_UTEST_SIZEOF(addresses<int>);
    
    int a0=0, a1=1, a2=2, a3=3, a4=4;
    
    addresses<int> ia1(a0);             Y_CHECK(1==ia1.size);
    addresses<int> ia2(a0,a1);          Y_CHECK(2==ia2.size);
    addresses<int> ia3(a0,a1,a2);       Y_CHECK(3==ia3.size);
    addresses<int> ia4(a0,a1,a2,a3);    Y_CHECK(4==ia4.size);
    addresses<int> ia5(a0,a1,a2,a3,a4); Y_CHECK(5==ia5.size);
    
    addresses<const int> ca1(a0);             Y_CHECK(1==ca1.size);
    addresses<const int> ca2(a0,a1);          Y_CHECK(2==ca2.size);
    addresses<const int> ca3(a0,a1,a2);       Y_CHECK(3==ca3.size);
    addresses<const int> ca4(a0,a1,a2,a3);    Y_CHECK(4==ca4.size);
    addresses<const int> ca5(a0,a1,a2,a3,a4); Y_CHECK(5==ca5.size);
    
    check_addr(ia1);
    check_addr(ia2);
    check_addr(ia3);
    check_addr(ia4);
    check_addr(ia5);

    check_addr(ca1);
    check_addr(ca2);
    check_addr(ca3);
    check_addr(ca4);
    check_addr(ca5);
    
}
Y_UTEST_DONE()


