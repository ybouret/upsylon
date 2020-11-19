
#include "y/associative/hash/link.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"

using namespace upsylon;

namespace
{


    static inline
    void doHashSlots(const size_t n)
    {
        static const size_t one = 1;
        hash_slots          hs(n);
        const size_t required  = hs.slots * sizeof(hash_meta_list);
        const size_t allocated = one << hs.sexp2;
        std::cerr << "n=" << n << std::endl;
        std::cerr << "\tslots     = " << hs.slots << std::endl;
        std::cerr << "\tsmask     = " << hs.smask << std::endl;
        std::cerr << "\tsexp2     = " << hs.sexp2 << std::endl;
        std::cerr << "\tbytes     = " << allocated << "/" << required << std::endl;
        Y_CHECK( allocated >= required );

        hash_meta_pool pool;
        try {
            int data = 1;

            for(size_t i=1+alea.leq(100);i>0;--i)
            {
                const size_t    hkey = alea.full<uint16_t>();
                hs[hkey].push_front( pool.query(hkey,&data) );
            }

            hs.dump();

            
            hs.to(pool);
        }
        catch(...)
        {
            hs.to(pool);
            throw;
        }

        pool.sort();
        std::cerr << "pool.size=" << pool.size << std::endl;
        std::cerr << std::endl;
    }
    

    template <typename KEY>
    class HNode : public object
    {
    public:

        explicit HNode(const KEY &k) throw() : next(0), prev(0), meta(0), key_(k)
        {
        }

        virtual ~HNode() throw()
        {

        }

        HNode          *next;
        HNode          *prev;
        hash_meta_node *meta;
        const KEY       key_;

        const KEY & key() const throw() { return key_; }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(HNode);
    };

    template <typename KEY>
    static inline void doHashLink()
    {
        typedef HNode<KEY>  Node;
        hash_link<KEY,Node> hl(10);

        vector<size_t> H;
        vector<KEY>    K;
        for(size_t i=10+alea.leq(20);i>0;--i)
        {
            const size_t    h = alea.leq(100);
            const KEY       k = support::get<KEY>();
            hash_meta_list *s = 0;

            if(hl.search_at(h,s,k))
            {
                
            }

            H << h;
            K << k;
        }


    }


}

Y_UTEST(hashed)
{
    doHashSlots(0);
    for(size_t n=1;n<=16;n<<=1)
    {
        doHashSlots(n);
    }

    doHashLink<uint16_t>();

}
Y_UTEST_DONE()


