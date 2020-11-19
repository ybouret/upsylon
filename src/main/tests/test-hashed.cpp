#include "y/associative/hash/znodes.hpp"
#include "y/associative/hash/link.hpp"
#include "y/associative/hash/buckets.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
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

    static inline void doTestBucket()
    {
        hash_bucket bucket;
        for(size_t i=1+alea.leq(100);i>0;--i)
        {
            bucket.push();
            if( alea.choice() )
                bucket.push_back(  hash_handle::acquire() );
            else
                bucket.push_front( hash_handle::acquire() );
        }
        alea.shuffle(bucket);
    }


    static inline void doTestBuckets(const size_t n)
    {
        std::cerr << "Buckets #" << n << std::endl;
        hash_bucket  pool;
        hash_buckets B(n);
        std::cerr << "\t->buckets=" << B.buckets << std::endl;
        std::cerr << "\t->__bmask=" << B.__bmask << std::endl;
        std::cerr << "\t->__bexp2=" << B.__bexp2 << std::endl;

        for(size_t i=1+alea.leq(100);i>0;--i)
        {
            const size_t hkey = alea.full<uint16_t>();
            pool.push();
            B[hkey].push_back( pool.query(hkey,0) );
        }

        B.dump();

        hash_buckets S(n/2);
        B.to(S);
        S.dump();

        std::cerr << std::endl;
    }

    struct zNode
    {
        zNode *next,*prev;
    };

    static inline void doTestZNodes()
    {
        std::cerr << "testing zNodes" << std::endl;
        hash_znodes<zNode> Z;
        for(size_t i=1+alea.leq(100);i>0;--i)
        {
            Z.push();
        }
        std::cerr << "#zombie=" << Z.size << std::endl;
        alea.shuffle(Z);

    }


}

Y_UTEST(hashed)
{
    Y_UTEST_SIZEOF(hash_handle);
    Y_UTEST_SIZEOF(hash_bucket);
    doTestBucket();
    doTestBuckets(0);
    for(size_t n=1;n<=16;n<<=1)
    {
        doTestBuckets(n);
    }

    doTestZNodes();

    return 0;
    doHashSlots(0);
    for(size_t n=1;n<=16;n<<=1)
    {
        doHashSlots(n);
    }

    doHashLink<uint16_t>();



}
Y_UTEST_DONE()


