
#include "y/memory/shack.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/string.hpp"

using namespace upsylon;

Y_UTEST(shack)
{
    memory::shack S;
    for(size_t i=0;i<=10000;i += alea.leq(100) )
    {

        S.acquire(i);
        Y_ASSERT(S.is_anonymous());
        Y_ASSERT(S.is_zeroed());
        for(size_t j=0;j<8;++j)
        {
            S.release();
            S.acquire(i);
        }
    }


    S.release();
    S.acquire(1);
    Y_CHECK(S.is_anonymous());
    Y_CHECK(S.tell()==0);
    S.make<string>() = "hello";
    Y_CHECK(S.is_cplusplus());
    Y_CHECK(S.is<string>());
    Y_CHECK(S.tell()==1);

    S.make<string>(7);
    Y_CHECK(S.is_cplusplus());
    Y_CHECK(S.is<string>());
    Y_CHECK(S.tell()==7);

    Y_UTEST_SIZEOF(memory::shack);

}
Y_UTEST_DONE()
