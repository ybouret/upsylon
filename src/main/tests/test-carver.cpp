#include "y/memory/carver.hpp"
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

    static inline void fill(core::list_of<block> &blocks,
                            memory::carver       &C,
                            const size_t nmax)
    {
        for(size_t i=0;i<nmax;++i)
        {
            block *b = new block();
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
    memory::carver C(0);

    core::list_of_cpp<block> blocks;

    fill(blocks,C,10);


}
Y_UTEST_DONE()

