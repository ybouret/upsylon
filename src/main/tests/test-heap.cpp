#include "y/ordered/heap.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/pooled.hpp"
#include "y/hashing/sha1.hpp"
using namespace upsylon;


Y_UTEST(heap)
{
    { heap<int> H; }
    { heap<int> H(0,as_capacity); }
    { heap<int> H(10,as_capacity); }

    vector<int>   data;
    heap<int>     H;
    heap<int,decreasing_comparator<int>,memory::pooled> D;
    for(size_t iter=0;iter<16;++iter)
    {
        data.free();
        D.release();
        H.release();
        for(size_t i=1+alea.leq(31);i>0;--i)
        {
            data << alea.partial<int>(12);
        }

        std::cerr << "Heap For " << data.size() << std::endl;
        for(size_t i=1;i<=data.size();++i)
        {
            H.enqueue( &data[i] );
            D.enqueue( &data[i] );
            std::cerr << " +(" << data[i] << "):";
            std::cerr << "[" << H.peek() << "] / [" << D.peek() << "]" << std::endl;
        }
        Y_CHECK( D.size() == H.size() );
        while( D.size() )
        {
            const int lhs = * H.extract();
            const int rhs = * D.extract();
            std::cerr << "[" << lhs << "] / [" << rhs << "]" << std::endl;
        }

    }
#if 0
    vector<int>   data;
    hashing::sha1 h;
    
    heap<int>   H;
    heap<int,decreasing_comparator<int>,memory::pooled> D;
    
    for(size_t iter=0;iter<16;++iter)
    {
        data.free();
        D.release();
        H.release();
        for(size_t i=1+alea.leq(31);i>0;--i)
        {
            data << alea.partial<int>(12);
        }
        
        std::cerr << "Heap For " << data.size() << std::endl;
        for(size_t i=1;i<=data.size();++i)
        {
            H.push( &data[i] );
            D.push( &data[i] );
            std::cerr << " +(" << data[i] << "):";
            std::cerr << "[" << H.peek();
            if(i>1)
            {
                std::cerr << "->" << H.second();
            }
            std::cerr << "] / [" << D.peek();
            if(i>1)
            {
                std::cerr << "->" << D.second();
            }

            std::cerr << "]" << std::endl;
        }

        {
            const digest h1 = h.md(H);
            heap<int> H2(H);
            const digest h2 = h.md(H2);
            std::cerr << "h1=" << h1 << std::endl;
            std::cerr << "h2=" << h2 << std::endl;
            Y_ASSERT(h1==h2);
        }

        std::cerr << H.size() << "/" << H.capacity();
        while( H.size() )
        {
            std::cerr << ":" << * H.pop();
        }
        std::cerr << std::endl;
        
        std::cerr << D.size() << "/" << D.capacity();
        while( D.size() )
        {
            std::cerr << ":" << * D.pop();
        }
        std::cerr << std::endl;
    }
#endif

}
Y_UTEST_DONE()

