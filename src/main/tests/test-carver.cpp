#include "y/memory/cblock.hpp"
#include "y/memory/pooled.hpp"
#include "y/memory/carver.hpp"
#include "y/utest/run.hpp"
#include "y/object.hpp"
#include "y/sort/merge.hpp"
#include "y/comparison.hpp"

using namespace upsylon;

namespace
{
    class block : public object
    {
    public:
        block *next;
        block *prev;
        void  *addr;
        size_t capa;
        size_t size;
        memory::allocator &crv;

        inline block( memory::allocator &C ) throw() : next(0), prev(0), addr(0), capa(0), size(0), crv(C)
        {
        }

        inline virtual ~block() throw()
        {
            if(addr)
            {
                assert(capa>0);
                crv.release(addr,capa);
            }
        }

        static inline int compare( const block *lhs, const block *rhs, void * )
        {
            return comparison::increasing(lhs->size,rhs->size);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(block);
    };

    static inline void fill(core::list_of<block> &blocks,
                            memory::allocator    &C,
                            const size_t          nmax)
    {
        while(blocks.size<nmax)
        {
            block *b = new block(C);
            try
            {
                b->capa  = 1+alea.leq(100);
                b->addr  = C.acquire(b->capa);
                b->size  = alea.leq(b->capa);
                uint8_t *p = static_cast<uint8_t *>(b->addr);
                for(size_t i=0;i<b->size;++i)
                {
                    p[i] = alea.full<uint8_t>();
                }
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
            std::cerr << "-- initial fill" << std::endl;
            fill(blocks,C,2048);
            alea.shuffle(blocks);
            {
                const size_t nhalf = blocks.size/2;
                while( blocks.size>nhalf )
                {
                    delete blocks.pop_back();
                }
            }

            std::cerr << "-- second fill" << std::endl;
            fill(blocks,C,2048);
            alea.shuffle(blocks);
            {
                const size_t nhalf = blocks.size/2;
                while( blocks.size>nhalf )
                {
                    delete blocks.pop_back();
                }
            }

            std::cerr << "-- sorting" << std::endl;
            merging<block>::sort(blocks, block::compare, NULL);
            size_t j=0;
            for(block *blk=blocks.head;blk;blk=blk->next)
            {
                assert(blk->addr);
                assert(blk->capa);
                assert(blk->size<=blk->capa);
                if( C.compact(blk->addr, blk->capa, blk->size ) )
                {
                    std::cerr << '+';
                }
                else
                {
                    std::cerr << '-';
                }
                if( 0 == (++j&63) ) std::cerr << std::endl;
            }
            std::cerr << std::endl;
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
