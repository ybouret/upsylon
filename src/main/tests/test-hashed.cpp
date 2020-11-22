#include "y/associative/hash/proto.hpp"
#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;

namespace
{


    static inline void doTestBucket()
    {
        std::cerr << "-- testing Bucket" << std::endl;
        hash_bucket bucket;
        for(size_t i=1+alea.leq(100);i>0;--i)
        {
            bucket.push();
            if( alea.choice() )
                bucket.push_back(  hash_meta::acquire() );
            else
                bucket.push_front( hash_meta::acquire() );
        }
        alea.shuffle(bucket);
        std::cerr << std::endl;
    }


    static inline void doTestBuckets(const size_t n)
    {
        std::cerr << "-- testing Buckets #" << n << std::endl;
        hash_bucket  pool;
        hash_buckets B(n);
        std::cerr << "\t->count=" << B.count << std::endl;
        std::cerr << "\t->bmask=" << B.bmask << std::endl;
        std::cerr << "\t->bexp2=" << B.bexp2 << std::endl;

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
        std::cerr << "-- testing zNodes" << std::endl;
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
        std::cerr << "-- testing zPairs" << std::endl;
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

    template <typename KEY,typename T>
    class KNode
    {
    public:
        KNode       *next;
        KNode       *prev;
        hash_meta   *meta;
        const KEY    _key;
        T            data;

        inline KNode(const KEY &k, const T &x) :
        next(0),
        prev(0),
        meta(0),
        _key(k),
        data(x)
        {
        }

        inline const KEY & key() const throw()
        {
            return _key;
        }


        inline ~KNode() throw() {}
        
        inline KNode( const KNode &node ) :
        next(0), prev(0), meta(0), _key(node._key), data(node.data)
        {
        }
        
        
    private:
        Y_DISABLE_ASSIGN(KNode);
    };

    template <typename KEY, typename T>
    void doTestTable()
    {
        std::cerr << "-- testing Table<" << type_name_of<KEY>() << "," << type_name_of<T>() << ">" << std::endl;

        typedef KNode<KEY,T> Node;
        hash_table<Node>     table;

        {
            const KEY    k = support::get<KEY>();
            const size_t h = alea.leq(100);
            hash_bucket *b = 0;
            Y_CHECK( !table.template search<KEY>(k,h,b)  );
            Y_CHECK( !table.template remove<KEY>(k,h)    );

            b = 0;
            const T tmp = support::get<T>();
            Y_CHECK( table.template insert<KEY>(k,h,tmp) );
            Y_CHECK( table.template search<KEY>(k,h,b)    );
            Y_CHECK( table.nodes.size == 1);
            table.free();
            Y_CHECK( table.nodes.size == 0);

            b=0;
            Y_CHECK( table.template insert<KEY>(k,h,tmp) );
            Y_CHECK( table.template search<KEY>(k,h,b)    );
            Y_CHECK( table.nodes.size == 1);
        }

        for(size_t cycles=0;cycles<16;++cycles)
        {
            table.release();
            const size_t    iterMax = 50+alea.leq(150);
            vector<KEY>     keys(iterMax,as_capacity);
            vector<size_t>  hkeys(iterMax,as_capacity);

            for(size_t iter=0;iter<iterMax;++iter)
            {
                const KEY    k   = support::get<KEY>();
                const size_t h   = alea.leq(100);
                const T      tmp = support::get<T>();
                if( table. template insert<KEY>(k,h,tmp) )
                {
                    keys  << k;
                    hkeys << h;
                }
            }

            std::cerr << "keys: " << keys.size() << std::endl;
            std::cerr << "load: " << table.load_factor() << std::endl;
            const size_t required = 8;
            std::cerr << "bucket for " << required << " : " << table.buckets_for_load_factor(required) << std::endl;

            if(cycles<=0)
            {
                std::cerr << "compact" << std::endl;
                table.pails.dump();
            }
            table.load_factor(required);
            if(cycles<=0)
            {
                std::cerr << "expanded" << std::endl;
                table.pails.dump();
            }
            std::cerr << "load: " << table.load_factor() << std::endl;
            Y_CHECK(table.load_factor() <= required );

            Y_CHECK(keys.size()==hkeys.size());
            Y_CHECK(keys.size()==table.nodes.size);

            const size_t   n = keys.size();
            vector<size_t> idx(n,0);
            for(size_t i=n;i>0;--i) idx[i] = i;
            alea.shuffle(*idx,n);
            for(size_t i=n;i>0;--i)
            {
                const size_t j    = idx[i];
                const KEY   &k    = keys[j];
                const size_t h    = hkeys[j];
                hash_bucket *b    = 0;
                Node        *node = table. template search<KEY>(k,h,b);
                Y_ASSERT(NULL!=node);
                Y_ASSERT(NULL!=node->meta);
                Y_ASSERT(h==node->meta->hkey);
                {
                    const hash_table<Node> &const_table = table;
                    b = 0;
                    Y_ASSERT(NULL!=const_table. template search<KEY>(k,h,b) );
                }
            }
            
            {
                hash_table<Node> table_copy(table);
                table.swap_with(table_copy);
            }
            
            alea.shuffle(*idx,n);
            for(size_t i=n;i>0;--i)
            {
                const size_t j    = idx[i];
                const KEY   &k    = keys[j];
                const size_t h    = hkeys[j];
                Y_ASSERT( table. template remove<KEY>(k,h) );
            }

        }

        std::cerr << std::endl;
    }
    
    template <typename KEY, typename T> class my_proto :
    public hash_proto<KEY,T,KNode<KEY,T>, key_hasher<KEY>, container>
    {
    public:
        typedef hash_proto<KEY,T,KNode<KEY,T>, key_hasher<KEY>, container> prototype;
        
        inline virtual ~my_proto() throw()
        {
        }
        
        inline explicit my_proto() throw(): prototype()
        {
        }
        
        static inline
        void doTest()
        {
            std::cerr << "-- testing Proto<" << type_name_of<KEY>() << "," << type_name_of<T>() << ">" << std::endl;
            my_proto    proto;
            vector<KEY> keys;
            for(size_t i=10+alea.leq(10);i>0;--i)
            {
                const KEY key = support::get<KEY>();
                
                if(true)
                {
                    keys << key;
                }
            }
            
            const size_t n = keys.size();
            alea.shuffle(*keys,n);
            for(size_t i=n;i>0;--i)
            {
                const KEY &key = keys[i];
                proto.search(key);
            }
            
            std::cerr << std::endl;
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(my_proto);
    };
    
    

}

Y_UTEST(hashed)
{
    Y_UTEST_SIZEOF(hash_meta);
    Y_UTEST_SIZEOF(hash_bucket);
    doTestBucket();
    doTestBuckets(0);
    for(size_t n=1;n<=16;n<<=1)
    {
        doTestBuckets(n);
    }

    doTestZNodes();
    doTestZPairs();
    doTestTable<int,int>();
    doTestTable<string,apq>();
    
    my_proto<uint8_t,int>::doTest();
    
}
Y_UTEST_DONE()


