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
    memory::vein v;

    for(size_t i=0;i<=v.max_size;++i)
    {
        size_t       bits = 0;
        const size_t len = memory::vein::bytes_for(i,bits);
        std::cerr << i << " \t-> " <<  len << " @bits=" << bits << std::endl;
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


}
Y_UTEST_DONE()

