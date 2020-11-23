#include "y/associative/hash/set.hpp"
#include "y/associative/hash/map.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"
#include "y/sort/heap.hpp"
#include "y/hashing/sha1.hpp"

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

        inline explicit KNode(const KEY &k, const T &x) :
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
        
        inline explicit KNode(const KNode &node) :
        next(0), prev(0), meta(0), _key(node._key), data(node.data)
        {
            std::cerr  << _key << '/';
        }
        
        
    private:
        //Y_DISABLE_COPY_AND_ASSIGN(KNode);
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
                Y_ASSERT(table_copy.nodes.size==table.nodes.size);
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



    class my_collection : public virtual collection
    {
    public:
        inline explicit my_collection() throw() : collection() {}
        inline virtual ~my_collection() throw() {}

    private:
        Y_DISABLE_COPY_AND_ASSIGN(my_collection);
    };

    template <typename KEY, typename T> class my_proto :
    public hash_proto<KEY,T,KNode<KEY,T>, key_hasher<KEY,hashing::sha1>, my_collection>
    {
    public:
        typedef hash_proto<KEY,T,KNode<KEY,T>, key_hasher<KEY,hashing::sha1>, my_collection> prototype;
        
        inline virtual ~my_proto() throw()
        {
        }
        
        inline explicit my_proto() throw(): prototype()
        {
        }

        inline bool insert(const KEY &key, const size_t &hkey, const T &tmp)
        {
            return this->table. template insert<KEY>(key,hkey,tmp);
        }

        static inline
        void doTest()
        {
            std::cerr << "-- testing Proto<" << type_name_of<KEY>() << "," << type_name_of<T>() << ">" << std::endl;
            my_proto    proto;
            vector<KEY> keys;
            for(size_t i=100+alea.leq(100);i>0;--i)
            {
                const KEY    key  = support::get<KEY>();
                const size_t hkey = proto.hash(key);
                const T      tmp  = support::get<T>();
                if(proto.insert(key,hkey,tmp))
                {
                    keys << key;
                }
            }
            std::cerr << "\t#keys=" << keys.size() << std::endl;
            proto.post_insert();
            Y_CHECK( proto.size() == keys.size() );
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

    template <typename KEY>
    class Dummy
    {
    public:
        const KEY myKey;

        inline   Dummy(const KEY &k) : myKey(k)
        {
        }

        inline Dummy(const Dummy &other) : myKey(other.myKey)
        {
        }

        inline ~Dummy() throw() {}

        inline const KEY & key() const throw() { return myKey; }

    private:
        Y_DISABLE_ASSIGN(Dummy);
    };

    template <typename KEY>
    static inline void doTestHashSet()
    {
        std::cerr << "-- testing HashSet<" << type_name_of<KEY>() << ">" << std::endl;

        typedef Dummy<KEY>   KDummy;
        hash_set<KEY,KDummy> db;
        vector<KEY>          keys;

        for(size_t i=10+alea.leq(10);i>0;--i)
        {
            const KEY    key = support::get<KEY>();
            const KDummy dum = KDummy(key);
            if( db.insert(dum) )
            {
                keys << key;
            }
        }
        std::cerr << "\t#keys=" << keys.size() << std::endl;
        Y_CHECK( keys.size() == db.size() );

        hsort(keys,comparison::increasing<KEY>);
        db.sort_keys_with(comparison::increasing<KEY>);
        std::cerr << "\tcomparing keys" << std::endl;
        {
            size_t                                   i=1;
            typename  hash_set<KEY,KDummy>::iterator it=db.begin();
            for(size_t j=keys.size();j>0;--j,++i,++it)
            {
                Y_ASSERT( keys[i] == it.get().key() );
            }
        }

        {
            // copy
            std::cerr << "\tcopy" << std::endl;
            const hash_set<KEY,KDummy> db2(db);
            Y_CHECK( db2.size() == db.size() );
            typename  hash_set<KEY,KDummy>::iterator       it=db.begin();
            typename  hash_set<KEY,KDummy>::const_iterator jt=db2.begin();
            for(size_t i=db.size();i>0;--i,++it,++jt)
            {
                Y_ASSERT( it.get().key() == jt.get().key() );
            }
        }

        {
            // assign
            std::cerr << "\tassign" << std::endl;
            hash_set<KEY,KDummy> db2(100,as_capacity);
            db2 = db;
            Y_CHECK( db2.size() == db.size() );
            typename  hash_set<KEY,KDummy>::iterator it=db.begin();
            typename  hash_set<KEY,KDummy>::iterator jt=db2.begin();
            for(size_t i=db.size();i>0;--i,++it,++jt)
            {
                Y_ASSERT( it.get().key() == jt.get().key() );
            }
        }

        

        std::cerr << std::endl;

    }


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
    my_proto<string,apq>::doTest();


    doTestHashSet<uint16_t>();
    doTestHashSet<string>();

}
Y_UTEST_DONE()


