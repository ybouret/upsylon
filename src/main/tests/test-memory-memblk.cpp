#include "y/ptr/cblock.hpp"
#include "y/ptr/cppblock.hpp"
#include "y/utest/run.hpp"

#include "y/string.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/memory/allocator/pooled.hpp"

using namespace upsylon;

namespace
{
    template <typename T,typename ALLOCATOR>
    void do_test()
    {
        for(size_t iter=0;iter<16;++iter)
        {
            const size_t required = alea.leq(1000);
            cblock<T,ALLOCATOR>   cblk( required ); Y_ASSERT(cblk.count==required);
            cppblock<T,ALLOCATOR> xblk( required ); Y_ASSERT(cblk.count==required);
            for(size_t i=0;i<cblk.count;++i)
            {
                (void) cblk[i];
                (void) xblk[i+1];
            }
        }
    }
    
    template <typename T>
    void do_tests()
    {
        do_test<T,memory::global>();
        do_test<T,memory::dyadic>();
        do_test<T,memory::pooled>();

    }
}

Y_UTEST(memblk)
{
   
    do_tests<char>();
    do_tests<double>();
    do_tests<string>();

}
Y_UTEST_DONE()

