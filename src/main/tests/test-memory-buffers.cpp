#include "y/memory/buffers.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

Y_UTEST(buffers)
{

    for(size_t i=0;i<=1000;i+=1+alea.leq(100))
    {
        memory::cblock blk(i);
        std::cerr << "query " << i << " => " << blk.length() << std::endl;

        memory::cppblock<uint8_t> blk8(i);
        std::cerr << "query " << i << " => " << blk8.length() << " => " << blk8.size() << std::endl;

        memory::cppblock<uint32_t> blk32(i);
        std::cerr << "query " << i << " => " << blk32.length() << " => " << blk32.size() << std::endl;

        std::cerr << std::endl;

    }

}
Y_UTEST_DONE()

