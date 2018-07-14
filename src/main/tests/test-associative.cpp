#include "y/associative/hash-table.hpp"
#include "y/hashing/key-hasher.hpp"
#include "y/utest/run.hpp"

using namespace upsylon;

namespace
{
    class hnode
    {
    public:
        hnode *next;
        hnode *prev;
        void  *meta;
        const size_t key;
        const size_t hkey;
        int          data;

        inline hnode(size_t k, size_t h, int i) throw() :
        next(0),
        prev(0),
        meta(0),
        key(k),
        hkey(h),
        data(i)
        {
            //std::cerr << "key=" << k << "=>h=" << h << std::endl;
        }

        inline ~hnode() throw()
        {
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(hnode);
    };
}

Y_UTEST(associative)
{
    key_hasher<size_t> H;

    {
        core::hash_table<hnode> htable0;
    }

    for(size_t n=0;n<=1000;n+=1+alea.leq(30))
    {
        core::hash_table<hnode> htable(n);
        std::cerr << "n=" << n << " -> items=" << htable.items <<  ", slots=" << htable.slots << ", max_load=" << double(htable.items)/max_of<size_t>(1,htable.slots) << std::endl;
        for(size_t i=1;i<=n;++i)
        {
            htable.insert<size_t,int>(i,H(i),alea.full<int>());
        }
        core::hash_table<hnode> hnew;
        hnew.swap_with(htable);
        assert(hnew.chain.size==n);
        
    }
}
Y_UTEST_DONE()

