#include "y/ordered/heap.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/allocator/dyadic.hpp"
using namespace upsylon;


Y_UTEST(heap)
{
    concurrent::singleton::verbose = true;
    
    { heap<int> H; }

    { heap<int> H(0,as_capacity); }

    { heap<int> H(10,as_capacity); }

    vector<int>   data;
    heap<int>     H;
    heap<int,decreasing_comparator<int>,memory::pooled> D;
    heap<int,increasing_comparator<int>,memory::dyadic> I;

    for(size_t iter=0;iter<16;++iter)
    {
        data.free();
        D.release();
        H.release();
        I.release();
        for(size_t i=1+alea.leq(31);i>0;--i)
        {
            data << alea.partial<int>(12);
        }

        std::cerr << "Heap For " << data.size() << std::endl;
        for(size_t i=1;i<=data.size();++i)
        {
            H.enqueue( &data[i] );
            D.enqueue( &data[i] );
            I.enqueue( &data[i] );
            std::cerr << " +(" << data[i] << "):";
            std::cerr << "[" << H.peek() << "] / [" << D.peek() << "]" << std::endl;
        }
        Y_CHECK( D.size() == H.size() );
        Y_CHECK( I.size() == D.size() );
        while( D.size() )
        {
            const int lhs = * H.extract();
            const int rhs = * D.extract();
            std::cerr << "[" << lhs << "] / [" << rhs << "]" << std::endl;
        }

    }


}
Y_UTEST_DONE()

