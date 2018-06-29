#include "y/object.hpp"
#include "y/utest/run.hpp"
#include "y/memory/cblock.hpp"
#include "y/os/rt-clock.hpp"

using namespace upsylon;

namespace
{
    struct block
    {
        void  *p0;
        void  *p1;
        size_t size;
    };
}

Y_UTEST(object)
{

    const size_t N = 16384;

    memory::cblock_of<block> org(N);
    block                   *blk=org.data;
    uint64_t   s=0;
    uint64_t   y=0;
    for(size_t iter=0;iter<16;++iter)
    {
        for(size_t i=0;i<N;++i)
        {
            block       &b = blk[i];
            const size_t n = (b.size = 1+alea.lt(2*Y_LIMIT_SIZE));

            {
                void    *p0   = 0;
                uint64_t mark = rt_clock::ticks();
                {
                    Y_LOCK(memory::global::access);
                    p0 = calloc(1,n);
                }
                s += (rt_clock::ticks()-mark);
                b.p0 = p0;
                if(!p0) throw exception("no memory for calloc");
            }

            {
                void    *p1   = 0;
                uint64_t mark = rt_clock::ticks();
                p1 = object:: operator new(n);
                y += (rt_clock::ticks()-mark);
                b.p1 = p1;
                assert(p1);
            }

        }

        alea.shuffle(blk,N);

        for(size_t i=0;i<N;++i)
        {
            block       &b = blk[i];
            const size_t n = b.size;
            {
                void *p0 = b.p0;
                uint64_t mark = rt_clock::ticks();
                {
                    Y_LOCK(memory::global::access);
                    free(p0);
                }
                s += (rt_clock::ticks()-mark);
                b.p0 = 0;
            }

            {
                void *p1 = b.p1;
                uint64_t mark = rt_clock::ticks();
                object::operator delete(p1,n);
                y += (rt_clock::ticks()-mark);
                b.p1 = 0;
            }
            b.size = 0;
        }
    }

    std::cerr << "s=" << s << std::endl;
    std::cerr << "y=" << y << std::endl;

}
Y_UTEST_DONE()

