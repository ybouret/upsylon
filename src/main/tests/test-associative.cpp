#include "y/associative/hash-container.hpp"
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
        const size_t key_;
        const size_t hkey;
        int          data;

        const size_t & key() const throw() { return key_; }

        inline hnode(size_t k, size_t h, int i) throw() :
        next(0),
        prev(0),
        meta(0),
        key_(k),
        hkey(h),
        data(i)
        {
            //std::cerr << "key=" << k << "=>h=" << h << std::endl;
        }

        inline hnode(const hnode &other) :
        next(0),
        prev(0),
        meta(0),
        key_(other.key_),
        hkey(other.hkey),
        data(other.data)
        {
        }


        inline ~hnode() throw()
        {
        }


    private:
        Y_DISABLE_ASSIGN(hnode);
    };
}

Y_UTEST(associative)
{

    key_hasher<size_t> H;

    {
        core::hash_table<hnode> htable0;
    }

    {
        for(size_t n=0;n<=1000;n+=1+alea.leq(30))
        {
            std::cerr << std::endl;
            core::hash_table<hnode> htable(n);
            for(size_t i=1;i<=2*n;++i)
            {
                htable.insert<size_t,int>(i,H(i),alea.full<int>());
            }
            core::hash_table<hnode> hnew;
            hnew.swap_with(htable);

            core::hash_table<hnode> hdup(hnew.items+alea.leq(100));
            hdup.duplicate(hnew);
        }
    }

#if 1
    { hash_container<size_t,int,hnode> hc; }
    { hash_container<size_t,int,hnode> hc(100,as_capacity); }
#endif


}
Y_UTEST_DONE()

