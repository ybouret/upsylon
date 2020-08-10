#include "y/memory/zblock.hpp"
#include "y/utest/run.hpp"

#include "y/memory/allocator/pooled.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/memory/allocator/dyadic.hpp"

using namespace upsylon;


namespace
{

    template <typename T,typename ALLOCATOR>
    static inline void doTest()
    {
        for(size_t i=1;i<=1000;i += 10 + alea.leq(10))
        {
            zblock<T,ALLOCATOR> blk(i);
        }
    }


    template <typename T>
    static inline void doTests()
    {
        doTest<T,memory::global>();
        doTest<T,memory::pooled>();
        doTest<T,memory::dyadic>();

    }

}

Y_UTEST(zblock)
{
    doTests<char>();
    
}
Y_UTEST_DONE()

