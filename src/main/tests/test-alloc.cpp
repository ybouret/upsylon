#include "y/memory/cblock.hpp"
#include "y/utest/run.hpp"
#include "y/memory/io.hpp"
#include <typeinfo>

using namespace upsylon;

namespace
{
    struct block
    {
        void  *addr;
        size_t size;
    };

    template <typename T>
    static void do_cblock()
    {
        const size_t desired = alea.leq(1000);
        memory::cblock_of<T> b(desired);
        std::cerr << "cblock_of<" << typeid(T).name() << ">" << std::endl;
        std::cerr << "desired: " << desired << std::endl;
        std::cerr << "size:    " << b.size  << std::endl;
        std::cerr << "bytes:   " << b.bytes << std::endl;

    }

}


Y_UTEST(alloc)
{
    memory::allocator &a = memory::global::instance();
    {
        static const size_t N = 100;
        block blk[N];
        for(size_t iter=0;iter<128;++iter)
        {
            for(size_t i=0;i<N;++i)
            {
                blk[i].size = alea.leq(100);
                blk[i].addr = a.acquire(blk[i].size);
            }
            alea.shuffle(blk,N);
            for(size_t i=0;i<N;++i)
            {
                a.release( blk[i].addr, blk[i].size );
            }
        }
    }
    
    {
        size_t len = 5 * sizeof(double);
        double *d = (double *)a.acquire(len);
        double *end = d+5;
        std::cerr << "delta=" << memory::io::delta(d,end) << std::endl;
        void *p = d;
        a.release(p,len);
    }
    
    {
        char buff[20];
        double *a = (double *)&buff[0];
        double *b = (double *)&buff[1];
        std::cerr << "delta=" << memory::io::delta(a,b) << std::endl;
    }

    do_cblock<uint8_t>();
    do_cblock<uint32_t>();
    do_cblock<int64_t>();

    
}
Y_UTEST_DONE()



