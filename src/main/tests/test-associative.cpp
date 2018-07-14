#include "y/associative/hash-table.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    class hnode
    {
    public:
        hnode *next;
        hnode *prev;

        inline hnode() throw() : next(0), prev(0), meta(0)
        {
        }

        inline ~hnode() throw()
        {
        }

        void *meta;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(hnode);
    };
}

Y_UTEST(associative)
{
    {
        core::hash_table<hnode> htable0;
    }

    for(size_t n=0;n<=1000;n+=1+alea.leq(30))
    {
        core::hash_table<hnode> htable(n);
        std::cerr << "n=" << n << " -> items=" << htable.items <<  ", slots=" << htable.slots << ", max_load=" << double(htable.items)/htable.slots << std::endl;
        core::hash_table<hnode> h0;
        h0.swap_with(htable);
    }
}
Y_UTEST_DONE()

