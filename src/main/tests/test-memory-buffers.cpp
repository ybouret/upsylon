#include "y/memory/cppblock.hpp"
#include "y/memory/cbuffer.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "support.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/type/rtti.hpp"

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

    template <typename T> static inline
    void testBuff()
    {
        std::cerr << "cbuffer<" << rtti::name_of<T>() << ">" << std::endl;
        for(size_t i=0;i<=1000;i+=1+alea.leq(100))
        {
            memory::cbuffer<T,memory::global> gbuf(i);
            std::cerr << "\tquery global " << std::setw(5) << i << " => " << gbuf.allocated << "/" << i *sizeof(T) << std::endl;

            memory::cbuffer<T,memory::pooled> pbuf(i);
            std::cerr << "\tquery pooled " << std::setw(5) << i << " => " << pbuf.allocated << "/" << i *sizeof(T) << std::endl;

            memory::cbuffer<T,memory::dyadic> dbuf(i);
            std::cerr << "\tquery dyadic " << std::setw(5) << i << " => " << dbuf.allocated << "/" << i *sizeof(T) << std::endl;
        }
        std::cerr << std::endl;
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

    Y_UTEST_SIZEOF(memory::cblock);
    Y_UTEST_SIZEOF(memory::cppblock<char>);
    Y_UTEST_SIZEOF(memory::cppblock<uint64_t>);

    testBuff<char>();
    testBuff<short>();
    

}
Y_UTEST_DONE()


