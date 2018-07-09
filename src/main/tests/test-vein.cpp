#include "y/memory/vein.hpp"
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
        inline explicit block() throw() : next(0), prev(0), addr(0), size(0) {}
        inline virtual ~block() throw() {}
    private:
        Y_DISABLE_COPY_AND_ASSIGN(block);
    };
}

Y_UTEST(vein)
{
    std::cerr << "-- Building vein..." << std::endl;
    memory::vein v;

    std::cerr << "-- Checking sizes..." << std::endl;
    for(size_t i=0;i<=v.max_size;++i)
    {
        size_t       bits = 0;
        const size_t len = memory::vein::bytes_for(i,bits);
        if(len<i) throw exception("invalid length for input=%lu", (unsigned long)i);
    }


    core::list_of_cpp<block> blocks;
    for(size_t i=0;i<=3*v.min_size;++i)
    {
        size_t n = i;
        void  *p = v.acquire(n);
        block *b = new block();
        b->addr = p;
        b->size = n;
        blocks.push_back(b);
    }
    std::cerr << "#blocks=" << blocks.size << std::endl;
    alea.shuffle(blocks);
    while(blocks.size)
    {
        block *b = blocks.pop_back();
        v.release(b->addr,b->size);
        delete b;
    }

    for(size_t iter=0;iter<128;++iter)
    {
        assert(0==blocks.size);
        for(size_t i=0;i<16000;++i)
        {
            size_t n = alea.leq( memory::vein::max_size );
            void  *p = v.acquire(n);
            block *b = new block();
            b->addr = p;
            b->size = n;
            blocks.push_back(b);
        }
        alea.shuffle(blocks);
        while(blocks.size)
        {
            block *b = blocks.pop_back();
            v.release(b->addr,b->size);
            delete b;
        }
    }

}
Y_UTEST_DONE()

