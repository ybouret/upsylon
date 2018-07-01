#include "y/memory/slice.hpp"
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
        inline block() throw() : next(0), prev(0), addr(0), size(0)
        {
        }
        inline virtual ~block() throw()
        {
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
            blocks.push_back(b);
        }
    }
}

Y_UTEST(slice)
{
    std::cerr << "sizeof(slice::block)=" << sizeof(memory::slice::block) << std::endl;
    core::list_of_cpp<block> blocks;

    for(size_t bs=100;bs<=10000;bs += 100 + alea.leq(100))
    {

        memory::cblock_of<char> buffer(Y_ALIGN_FOR_ITEM(memory::slice::block,bs));
        std::cerr << "#bytes=" << buffer.bytes << std::endl;
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
}
Y_UTEST_DONE()
