#include "y/associative/hash/table.hpp"
#include "y/associative/hash/buckets.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"

using namespace upsylon;

namespace
{


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
        std::cerr << std::endl;
    }

    static inline void doTestZPairs()
    {
        std::cerr << "testing zPairs" << std::endl;
        hash_zpairs<zNode> Z;

        for(size_t i=1+alea.leq(100);i>0;--i)
        {
            Z.push();
        }

        alea.shuffle(Z.hlist);
        alea.shuffle(Z.zlist);

        std::cerr << "Z.size=" << Z.size() << std::endl;
        hash_bucket bucket;
        while( Z.size() )
        {
            bucket.push_back( Z.query(alea.leq(100) ) );
        }

        alea.shuffle(bucket);
        while(bucket.size)
        {
            Z.store(bucket.pop_back());
        }
        std::cerr << std::endl;


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
    doTestZPairs();
    

}
Y_UTEST_DONE()


