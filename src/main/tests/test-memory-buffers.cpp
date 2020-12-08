#include "y/memory/cppblock.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

using namespace upsylon;

namespace {

    template <typename T> static inline
    void testBlock( memory::cppblock<T> &blk )
    {
        for(size_t i=blk.size();i>0;--i)
        {
            blk[i] = support::get<T>();
        }
        alea.shuffle(*blk,blk.size());
        
    }
}

Y_UTEST(buffers)
{

    for(size_t i=0;i<=1000;i+=1+alea.leq(100))
    {
        memory::cblock blk(i);
        std::cerr << "query " << i << " => " << blk.length() << " => " << blk.count<int16_t>() << " int16_t " << std::endl;
        {
            memory::cblock  z;
            z.swap_with(blk);
        }

        memory::cppblock<uint8_t> blk8(i);
        std::cerr << "query " << i << " => " << blk8.length() << " => " << blk8.size() << std::endl;
        {
            memory::cppblock<uint8_t> z;
            z.swap_with(blk8);
            Y_ASSERT(z.size()>0);
            testBlock(z);
        }

        memory::cppblock<uint32_t> blk32(i);
        std::cerr << "query " << i << " => " << blk32.length() << " => " << blk32.size() << std::endl;
        {
            memory::cppblock<uint32_t> z;
            z.swap_with(blk32);
            Y_ASSERT(z.size()>0);
            testBlock(z);
        }
        std::cerr << std::endl;

    }

}
Y_UTEST_DONE()


