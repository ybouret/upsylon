#include "y/memory/buffers.hpp"
#include "y/utest/run.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/allocator/dyadic.hpp"

using namespace upsylon;

Y_UTEST(buffers)
{
    for(size_t iter=0;iter<32;++iter)
    {
        const size_t n1 = alea.leq(1000);
        memory::buffer_of<char,memory::pooled>    b1( n1 );
        Y_CHECK(n1==b1.size);
        Y_CHECK(sizeof(char)*n1==b1.length());

        const size_t n2 = alea.leq(1000);
        memory::buffer_of<int64_t,memory::global> b2( n2 );
        Y_CHECK(n2==b2.size);
        Y_CHECK(sizeof(int64_t)*n2==b2.length());


        const size_t n3 = alea.leq(1000);
        memory::buffer_of<int64_t,memory::dyadic> b3( n3 );
        Y_CHECK(n3==b3.size);
        Y_CHECK(sizeof(int64_t)*n3==b3.length());
    }


}
Y_UTEST_DONE()

