#include "y/ordered/heap.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;


Y_UTEST(heap)
{
    vector<int> data;

    for(size_t i=0;i<10;++i)
    {
        data << i;
    }

    alea.shuffle(*data,data.size());
    std::cerr << "data=" << data << std::endl;
    
    heap<int>   H;
    for(size_t i=1;i<=data.size();++i)
    {
        std::cerr << "inserting " << data[i] << std::endl;
        H.insert( &data[i] );
        std::cerr << "|_peek=" << H.peek() << std::endl;
    }

    while( H.size() )
    {
        std::cerr << "extract: " << * H.pop() << std::endl;
    }

}
Y_UTEST_DONE()

