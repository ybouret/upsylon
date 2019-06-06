#include "y/ordered/heap.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/pooled.hpp"

using namespace upsylon;


Y_UTEST(heap)
{
    vector<int> data;

    
    heap<int>   H;
    heap<int,decreasing_comparator<int>,memory::pooled> D;
    
    for(size_t iter=0;iter<16;++iter)
    {
        data.free();
        D.free();
        H.free();
        for(size_t i=1+alea.leq(31);i>0;--i)
        {
            data << alea.partial<int>(12);
        }
        
        std::cerr << "Heap For " << data.size() << std::endl;
        for(size_t i=1;i<=data.size();++i)
        {
            H.push( &data[i] );
            D.push( &data[i] );
            std::cerr << " +(" << data[i] << "):" << H.peek() << "/" << D.peek() << std::endl;
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

}
Y_UTEST_DONE()

