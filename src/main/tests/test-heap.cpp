#include "y/ordered/heap.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;


Y_UTEST(heap)
{
    vector<int> data;

    for(size_t i=0;i<20;++i)
    {
        data << i;
    }


    heap<int>   H;

    for(size_t iter=0;iter<16;++iter)
    {
        alea.shuffle(*data,data.size());
        for(size_t i=1;i<=data.size();++i)
        {
            H.insert( &data[i] );
            std::cerr << " +(" << data[i] << "):" << H.peek();
        }
        std::cerr << std::endl;

        while( H.size() )
        {
            std::cerr << " " << * H.pop();
        }
        std::cerr << std::endl;
    }

}
Y_UTEST_DONE()

