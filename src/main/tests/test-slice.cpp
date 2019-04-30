#include "y/memory/slice.hpp"
#include "y/memory/cblock.hpp"
#include "y/utest/run.hpp"
#include "y/sort/merge.hpp"
#include "y/comparison.hpp"
#include "y/object.hpp"

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
        inline block() throw() : next(0), prev(0), addr(0), capa(0), size(0)
        {
        }

        inline virtual ~block() throw()
        {
            if(addr)
            {
                assert(capa);
                (void) memory::slice::release(addr,capa);
            }
        }

        static inline int compare( const block *lhs, const block *rhs, void * )
        {
            return comparison::increasing(lhs->capa,rhs->capa);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(block);
    };

    static inline void fill( core::list_of<block> &blocks, memory::slice &s )
    {
        while(true)
        {
            block *b = new block();
            b->capa  = 1+alea.leq(100);
            b->addr  = s.acquire(b->capa);
            if(!b->addr)
            {
                delete b;
                break;
            }
            b->size = alea.leq(b->capa);
#if 1
            uint8_t *p = static_cast<uint8_t *>(b->addr);
            for(size_t i=0;i<b->size;++i)
            {
                p[i] = alea.full<uint8_t>();
            }
#endif
            blocks.push_back(b);
        }
    }
}

Y_UTEST(slice)
{
    std::cerr << "sizeof(slice::block)=" << sizeof(memory::slice::block) << std::endl;

    {
        std::cerr << "<Testing (de)allocation>" << std::endl;
        core::list_of_cpp<block> blocks;
        for(size_t bs=100;bs<=10000;bs += 100 + alea.leq(100))
        {

            memory::cblock_of<char> buffer(Y_ALIGN_FOR_ITEM(memory::slice::block,bs));
            std::cerr << "/#bytes=" << buffer.bytes;
            memory::slice s(buffer.data,buffer.bytes);

            for(size_t iter=0;iter<32;++iter)
            {
                fill(blocks,s);
                const size_t nhalf = blocks.size/2;
                alea.shuffle(blocks);
                while( blocks.size>nhalf )
                {
                    delete blocks.pop_back();
                }
                fill(blocks,s);
                alea.shuffle(blocks);
                blocks.release();
            }
        }
        std::cerr << std::endl;
        std::cerr << "<Testing (de)allocation/>" << std::endl;
    }

    if(false)
    {
        std::cerr << "<Testing re-allocation>" << std::endl;
        for(size_t iter=0;iter<32;++iter)
        {
            const size_t source_bs = 10000+alea.leq(10000);
            const size_t target_bs = source_bs+alea.leq(10000);

            memory::cblock_of<char> source_buffer(Y_ALIGN_FOR_ITEM(memory::slice::block,source_bs));
            memory::cblock_of<char> target_buffer(Y_ALIGN_FOR_ITEM(memory::slice::block,target_bs));

            memory::slice source(source_buffer.data, source_buffer.bytes);
            memory::slice target(target_buffer.data, target_buffer.bytes);

            core::list_of_cpp<block> source_blocks;
            core::list_of_cpp<block> target_blocks;


            fill(source_blocks,source);
            merging<block>::sort(source_blocks, block::compare, NULL);

            std::cerr << "#source=" << source_blocks.size << "/";

            while(source_blocks.size)
            {
                block         *blk   = source_blocks.pop_back();
                const uint32_t old32 = crc32(blk->addr,blk->size);
                if( target.receive(blk->addr, blk->capa, blk->size) )
                {
                    target_blocks.push_back(blk);
                    Y_ASSERT( crc32(blk->addr,blk->size) == old32 );
                }
                else
                {
                    source_blocks.push_back(blk);
                    break;
                }
            }

            target_blocks.release();
            
        }
        std::cerr << std::endl;


        std::cerr << "<Testing re-allocation/>" << std::endl;
    }
    std::cerr << "sizeof(memory::slice)=" << sizeof(memory::slice) << std::endl;
}
Y_UTEST_DONE()
