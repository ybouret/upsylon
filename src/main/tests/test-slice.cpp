#include "y/memory/slice.hpp"
#include "y/memory/cblock.hpp"
#include "y/utest/run.hpp"
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
        size_t size;
        inline block() throw() : next(0), prev(0), addr(0), size(0)
        {
        }
        inline virtual ~block() throw()
        {
        }

        static inline int compare( const block *lhs, const block *rhs, void * )
        {
            return comparison::increasing(lhs->size,rhs->size);
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(block);
    };

    static inline void fill( core::list_of<block> &blocks, memory::slice &s )
    {
        while(true)
        {
            block *b = new block();
            b->size  = 1+alea.leq(100);
            b->addr  = s.acquire(b->size);
            if(!b->addr)
            {
                delete b;
                break;
            }
            uint8_t *p = static_cast<uint8_t *>(b->addr);
            for(size_t i=0;i<b->size;++i)
            {
                p[i] = alea.full<uint8_t>();
            }
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
                    block *b = blocks.pop_back();
                    memory::slice::release(b->addr,b->size);
                    delete b;
                }
                fill(blocks,s);
                alea.shuffle(blocks);
                while(blocks.size>0)
                {
                    block *b = blocks.pop_back();
                    memory::slice::release(b->addr,b->size);
                    delete b;
                }
            }
        }
        std::cerr << std::endl;
        std::cerr << "<Testing (de)allocation/>" << std::endl;
    }

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
                size_t new_size = source_blocks.tail->size;
                void  *new_addr = target.receive(source_blocks.tail->addr,new_size);
                if(!new_addr) break;
                const uint32_t crc32_old = crc32(source_blocks.tail->addr,source_blocks.tail->size);
                const uint32_t crc32_new = crc32(new_addr,source_blocks.tail->size);
                Y_ASSERT(crc32_new==crc32_old);
                block *b = new block();
                b->addr = new_addr;
                b->size = new_size;
                target_blocks.push_back(b);
                source.release(source_blocks.tail->addr,source_blocks.tail->size);
                delete source_blocks.pop_back();
            }

            while(target_blocks.size>0)
            {
                block *b = target_blocks.pop_back();
                memory::slice::release(b->addr,b->size);
                delete b;
            }
        }
        std::cerr << std::endl;


        std::cerr << "<Testing re-allocation/>" << std::endl;
    }

}
Y_UTEST_DONE()
