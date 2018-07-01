#include "y/memory/slice.hpp"
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
}

Y_UTEST(slice)
{
    std::cerr << "sizeof(slice::block)=" << sizeof(memory::slice::block) << std::endl;

    char buffer[1000];
    memory::slice s(buffer,sizeof(buffer));
    s.display();

    core::list_of_cpp<block> blocks;

    while(true)
    {
        block *b = new block();
        b->size  = 1;
        b->addr  = s.acquire(b->size);
        if(!b->addr)
        {
            delete b;
            break;
        }
        blocks.push_back(b);
    }
    s.display();

    std::cerr << "shuffle #blocks=" << blocks.size << std::endl;
    const size_t nhalf = blocks.size/2;
    alea.shuffle(blocks);
    while( blocks.size > nhalf )
    {
        block *b = blocks.pop_back();
        memory::slice::release(b->addr,b->size);
        delete b;
    }
    s.display();


}
Y_UTEST_DONE()
