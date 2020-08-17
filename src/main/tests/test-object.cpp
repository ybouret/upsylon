#include "y/object.hpp"
#include "y/utest/run.hpp"
#include "y/memory/zblock.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/os/real-time-clock.hpp"
#include <cstdlib>

using namespace upsylon;

namespace
{
    struct block_t
    {
        void  *addr;
        size_t size;
    };
}

Y_UTEST(object0)
{
    size_t N    = 16384;
    size_t Iter = 16;

    if(argc>1) N    = atol(argv[1]);
    if(argc>2) Iter = atol(argv[2]);

    zblock<block_t,memory::global> org(N);
    block_t        *blk = *org;
    uint64_t        s   = 0;
    for(size_t iter=0;iter<Iter;++iter)
    {
        for(size_t i=0;i<N;++i)
        {
            block_t     &b = blk[i];
            const size_t n = (b.size = 1+alea.lt(2*Y_LIMIT_SIZE));

            {
                void    *addr   = 0;
                uint64_t mark = real_time_clock::ticks();
                {
                    Y_LOCK(memory::global::access);
                    addr = calloc(1,n);
                }
                s += (real_time_clock::ticks()-mark);
                b.addr = addr;
                if(!addr) throw exception("no memory for calloc");
            }
        }

        alea.shuffle(blk,N);
        for(size_t i=0;i<N;++i)
        {
            block_t     &b = blk[i];
            //const size_t n = b.size;
            {
                void *addr = b.addr;
                uint64_t mark = real_time_clock::ticks();
                {
                    Y_LOCK(memory::global::access);
                    free(addr);
                }
                s += (real_time_clock::ticks()-mark);
            }
            b.addr = 0;
            b.size = 0;
        }

    }
    std::cerr << "time0=" << double(s)/double(N*Iter) << std::endl;

}
Y_UTEST_DONE()

Y_UTEST(objectY)
{
    size_t N    = 16384;
    size_t Iter = 16;

    if(argc>1) N    = atol(argv[1]);
    if(argc>2) Iter = atol(argv[2]);

    zblock<block_t,memory::global> org(N);
    block_t        *blk= *org;
    uint64_t        s  = 0;
    for(size_t iter=0;iter<Iter;++iter)
    {
        for(size_t i=0;i<N;++i)
        {
            block_t     &b = blk[i];
            const size_t n = (b.size = 1+alea.lt(2*Y_LIMIT_SIZE));

            {
                void    *addr   = 0;
                uint64_t mark = real_time_clock::ticks();
                {
                    addr = object::operator new(n);
                }
                s += (real_time_clock::ticks()-mark);
                b.addr = addr;
                if(!addr)
                {
                    throw exception("no memory for calloc");
                }
            }
        }

        alea.shuffle(blk,N);
        for(size_t i=0;i<N;++i)
        {
            block_t     &b = blk[i];
            const size_t n = b.size;
            {
                void    *addr = b.addr;
                uint64_t mark = real_time_clock::ticks();
                {
                    object::operator delete(addr,n);
                }
                s += (real_time_clock::ticks()-mark);
            }
            b.addr = 0;
            b.size = 0;
        }

    }
    std::cerr << "timeY=" << double(s)/double(N*Iter) << std::endl;

}
Y_UTEST_DONE()


