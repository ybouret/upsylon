#include "y/memory/cblock.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/carver.hpp"
#include "y/utest/run.hpp"
#include "y/object.hpp"
#include "y/sort/merge.hpp"
#include "y/comparison.hpp"
#include <typeinfo>

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
                alea.fill(b->addr,b->size);
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
        for(size_t iter=0;iter<2;++iter)
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

            std::cerr << "-- compacting" << std::endl;
            size_t j=0;
            for(block *blk=blocks.head;blk;blk=blk->next)
            {
                assert(blk->addr);
                assert(blk->capa);
                assert(blk->size<=blk->capa);
                const uint32_t blk_crc32 = crc32(blk->addr,blk->size);
                if( C.compact(blk->addr, blk->capa, blk->size ) )
                {
                    Y_ASSERT(crc32(blk->addr,blk->size)==blk_crc32);
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

namespace
{
    template <typename T>
    class t_block : public object
    {
    public:
        T     *addr;
        size_t count;
        size_t bytes;
        size_t size;
        t_block *next;
        t_block *prev;

        explicit t_block() throw() : addr(0), count(0), bytes(0), size(0), next(0), prev(0)
        {
        }

        virtual ~t_block() throw()
        {
            if(addr)
            {
                memory::pooled::location().release_as(addr,count,bytes);
            }
            size = 0;
        }

        void create()
        {
            count = 1+alea.leq(100);
            addr  = memory::pooled::instance().acquire_as<T>(count,bytes);
            size  = alea.leq(count);
            alea.fill(addr,size*sizeof(T));
        }

        static inline int compare( const t_block *lhs, const t_block *rhs, void * )
        {
            return comparison::increasing(lhs->size,rhs->size);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(t_block);
    };

    template <typename T>
    void do_test_pooled(const size_t n)
    {
        std::cerr << "do_test[" << typeid(T).name() << "]" << std::endl;
        core::list_of_cpp< t_block<T> > t_list;
        memory::pooled                 &mgr = memory::pooled::instance();

        while(t_list.size<=n)
        {
            t_list.push_back( new t_block<T>() )->create();
        }
        alea.shuffle(t_list);
        const size_t nh = t_list.size/2;
        while( t_list.size > nh )
        {
            delete t_list.pop_back();
        }
        merging< t_block<T> >::sort(t_list,t_block<T>::compare,NULL);

        size_t j=0;
        for(t_block<T> *blk=t_list.head;blk;blk=blk->next)
        {
            const uint32_t blk32 = crc32(blk->addr,blk->size*sizeof(T));
            if( mgr.compact_as<T>(blk->addr, blk->count, blk->bytes, blk->size ) )
            {
                std::cerr << '+';
                Y_ASSERT(crc32(blk->addr,blk->size*sizeof(T))==blk32);
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

Y_UTEST(pooled)
{
    memory::pooled          &P  = memory::pooled::instance();
    core::list_of_cpp<block> blocks;

    if(P.exists())
    {
        for(size_t iter=0;iter<8;++iter)
        {
            std::cerr << "-- initial fill" << std::endl;
            fill(blocks,P,2048);
            alea.shuffle(blocks);
            {
                const size_t nhalf = blocks.size/2;
                while( blocks.size>nhalf )
                {
                    delete blocks.pop_back();
                }
            }

            std::cerr << "-- second fill" << std::endl;
            fill(blocks,P,2048);
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
                const uint32_t pooled_crc32 = crc32(blk->addr,blk->size);
                if( P.compact(blk->addr, blk->capa, blk->size ) )
                {
                    Y_ASSERT(crc32(blk->addr,blk->size)==pooled_crc32);;
                    std::cerr << '+';
                }
                else
                {
                    std::cerr << '-';
                }
                if( 0 == (++j&63) ) std::cerr << std::endl;
            }
            std::cerr << std::endl;
            blocks.release();
        }
    }

    do_test_pooled<uint8_t>(5000);
    do_test_pooled<uint16_t>(5000);
    do_test_pooled<uint32_t>(5000);
    do_test_pooled<uint64_t>(5000);

}
Y_UTEST_DONE()
