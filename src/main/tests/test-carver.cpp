#include "y/memory/pooled.hpp"
#include "y/memory/cblock.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    class block : public object
    {
    public:
        block *next;
        block *prev;
        void  *addr;
        size_t size;
        memory::carver &crv;

        inline block( memory::carver &C ) throw() : next(0), prev(0), addr(0), size(0), crv(C)
        {
        }

        inline virtual ~block() throw()
        {
            if(addr)
            {
                assert(size>0);
                crv.release(addr,size);
            }
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(block);
    };

    static inline void fill(core::list_of<block> &blocks,
                            memory::carver       &C,
                            const size_t          nmax)
    {
        while(blocks.size<nmax)
        {
            block *b = new block(C);
            try
            {
                b->size  = 1+alea.leq(100);
                b->addr  = C.acquire(b->size);

                blocks.push_back(b);
            }
            catch(...)
            {
                delete b;
                throw;
            }
        }
    }

}

Y_UTEST(carver)
{

    for(size_t bs=32;bs<=4096;bs*=2)
    {
        std::cerr << "carver" << bs << std::endl;
        memory::carver C(bs);
        for(size_t iter=0;iter<8;++iter)
        {
            core::list_of_cpp<block> blocks;
            fill(blocks,C,2048);
            const size_t nhalf = blocks.size/2;
            alea.shuffle(blocks);
            while( blocks.size>nhalf )
            {
                delete blocks.pop_back();
            }
            fill(blocks,C,2048);
            alea.shuffle(blocks);
        }
    }

}
Y_UTEST_DONE()

Y_UTEST(pooled)
{
    memory::pooled          &P  = memory::pooled::instance();
    core::list_of_cpp<block> blocks;

    if(P.exists())
    {
        std::cerr << "pooled memory is ok" << std::endl;
        for(size_t iter=0;iter<8;++iter)
        {
            fill(blocks,P,5000);
            const size_t nhalf = blocks.size/2;
            alea.shuffle(blocks);
            while( blocks.size>nhalf )
            {
                delete blocks.pop_back();
            }
            fill(blocks,P,2048);
            alea.shuffle(blocks);
            blocks.release();
        }
    }
}
Y_UTEST_DONE()
