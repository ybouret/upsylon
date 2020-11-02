#include "y/associative/prefix/depot.hpp"
#include "y/associative/prefix/tree.hpp"

#include "y/utest/run.hpp"
#include "y/utest/sizeof.hpp"
#include "y/type/spec.hpp"
#include "y/ptr/auto.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/memory/buffers.hpp"
#include "y/ios/icstream.hpp"
#include "y/counting/comb.hpp"
#include "y/counting/perm.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    static inline void dispText()
    {
        std::cerr << type_name_of<T>() << std::endl;
        for(size_t i=0;i<8;++i)
        {
            const T x = alea.full<T>();
            std::cerr << '\t' << prefix_::code_to_text(x) << std::endl;
        }
        std::cerr << "in_use_addr: " << (void*) ( prefix_data<T>::in_use_addr() ) << std::endl;
    }

    template <typename CODE, typename T>
    static inline void _disp(const bool save=false)
    {
        typedef prefix_node<CODE,T> node_type;
        std::cerr << "node<" << type_name_of<CODE>() << "," << type_name_of<T>() << "> : " << sizeof(node_type) << std::endl;

        auto_ptr<node_type>            root( new node_type(0,0,0) );
        typename node_type::list_t    &leaves = root->leaves;
        for(size_t i=1+alea.leq(10);i>0;--i)
        {
            leaves.push_back( new node_type(0,CODE(i),0) );
            leaves.tail->frequency = alea.range<size_t>(1,1000);
        }
        root->optimize();
        for(const node_type *node = leaves.head;node;node=node->next)
        {
            if(node->next)
            {
                Y_ASSERT(node->frequency>=node->next->frequency);
            }
        }
        if(save)
        {
            root->graphViz("node.dot");
        }
    }

    template <typename T>
    static inline void disp()
    {
        _disp<uint8_t,T>();
        _disp<uint16_t,T>();
        _disp<uint32_t,T>();
        _disp<uint64_t,T>();
    }

    template <typename NODE> static inline
    bool doSomething(const NODE *node, size_t &count)
    {
        ++count;
        memory::cppblock<typename NODE::code_t> blk(node->depth);
        node->encode(blk);
        std::cerr << "node #" << count << " : " << blk << std::endl;
        return true;
    }
    
    
    template <typename CODE, typename T>
    class Stem  : public prefix_stem<CODE,T>
    {
    public:
        typedef prefix_stem<CODE,T>           stem_type;
        typedef typename stem_type::node_type node_type;
        
        explicit Stem() : stem_type() {}
        
        virtual ~Stem() throw() {}
        
        Stem(const Stem &stem ) : stem_type()
        {
            this->duplicate(stem);
        }
        
        
        template <typename SEQUENCE>
        bool insert(SEQUENCE &seq, T *data)
        {
            return this->grow(seq,data);
        }
        
        inline void remove(const node_type *node) throw()
        {
            this->pull( (node_type *)node );
        }
        
        
    private:
        Y_DISABLE_ASSIGN(Stem);
    };
    
    template <typename CODE, typename T>
    void doStem()
    {
        std::cerr << "-- stem<" << type_name_of<CODE>() << "," << type_name_of<T>() << ">" << std::endl;
        typedef Stem<CODE,T>                  stem_type;
        typedef typename stem_type::node_type node_type;
        typedef vector<CODE> path_type;
        
        stem_type       stem;
        list<T>         data;
        list<path_type> paths;
        size_t          count = 0;
        
        std::cerr << "building:[";
        for(size_t iter=80;iter>0;--iter)
        {
            vector<CODE> path;
            for(size_t i=1+alea.leq(4);i>0;--i)
            {
                path << alea.range<CODE>('a','d');
            }
            data << support::get<T>();
            const bool ans = stem.insert(path, & data.back() );
            if(ans)
            {
                std::cerr << "+";
                ++count;
                Y_ASSERT(stem.tell()==count);
                paths << path;
            }
            else
            {
                std::cerr << "-";
                data.pop_back();
            }
        }
        std::cerr << "]" << std::endl;
        stem.get_root().graphViz("stem.dot");
        Y_CHECK(count==stem.tell());
#if 1
        std::cerr << "duplicate" << std::endl;
        {
            stem_type stem2(stem);
            Y_CHECK(stem2.similar_to(stem));
        }
#endif
        size_t total = 0;
        stem.for_each(doSomething<node_type>,total);
        
        alea.shuffle( *paths );
        std::cerr << "removing:[";
        while(paths.size())
        {
            const path_type &path = paths.back();
            const node_type *node = stem.find( path.begin(), path.size() );
            Y_ASSERT(node);
            Y_ASSERT(node->addr);
            Y_ASSERT(node->depth==path.size());
            memory::cppblock<CODE> blk(node->depth);
            node->encode(blk);
            for(size_t i=1;i<=node->depth;++i)
            {
                Y_ASSERT(blk[i]==path[i]);
            }
            paths.pop_back();
            stem.remove(node);
            Y_ASSERT(stem.tell()==paths.size());
            std::cerr << "-";
        }
        std::cerr << "]" << std::endl;
    }


    
    template <typename CODE> static inline
    void doDepot()
    {
        std::cerr << "-- depot<" << type_name_of<CODE>() << ">" << std::endl;
        typedef prefix_depot<CODE>            stem_type;
        //typedef typename stem_type::node_type node_type;
        typedef vector<CODE>                  path_type;
        
        stem_type       stem;
        list<path_type> paths;
        size_t          count = 0;
        
        std::cerr << "building:[";
        for(size_t iter=80;iter>0;--iter)
        {
            vector<CODE> path;
            for(size_t i=1+alea.leq(4);i>0;--i)
            {
                path << alea.range<CODE>('a','d');
            }
            if(stem.insert(path))
            {
                std::cerr << "+";
                ++count;
                Y_ASSERT(stem.tell()==count);
                paths << path;
                Y_ASSERT( stem.has(path) );
            }
            else
            {
                std::cerr << "-";
            }
        }
        std::cerr << "]" << std::endl;
        
        std::cerr << "duplicate" << std::endl;
        {
            stem_type stem2(stem);
            Y_CHECK(stem2==stem);
        }
        
        alea.shuffle( *paths );
        std::cerr << "removing:[";
        while(paths.size())
        {
            const path_type &path = paths.back();
            Y_ASSERT(stem.remove(path));
            Y_ASSERT(!stem.has(path));
            paths.pop_back();
            Y_ASSERT(stem.size()==paths.size());
            std::cerr << "-";
        }
        std::cerr << "]" << std::endl;
    }
    
    static inline
    void doStrings(const char *fileName)
    {
        std::cerr << "-- strings" << std::endl;
        prefix_depot<char> db;
        list<string>       ls(32000,as_capacity);
        
        std::cerr << "[";
        {
            ios::icstream fp(fileName);
            string        line;
            while(fp.gets(line))
            {
                if(db.insert(line))
                {
                    ls << line;
                }
                if( 0 == (ls.size()%1024) )
                {
                    (std::cerr << ".").flush();
                }
            }

        }
        std::cerr << "]" << std::endl;
        std::cerr << "#db=" << db.size() << "/" << ls.size() << std::endl;
    }

    static inline void testComb()
    {
        std::cerr << "-- testing combinations" << std::endl;
        prefix_depot<size_t> depo;
        for(size_t n=1;n<=8;++n)
        {
            for(size_t k=1;k<=n;++k)
            {
                depo.free();
                combination comb(n,k);
                for( comb.boot(); comb.good(); comb.next() )
                {
                    Y_ASSERT(depo.insert(comb));
                }
                //const string fn = vformat("comb_%u_%u.dot", (unsigned)n, (unsigned)k );
                //depo.get_root().graphViz(fn);
            }
        }
    }
    
    static inline void testPerm()
    {
        std::cerr << "-- testing permutations" << std::endl;
        prefix_depot<size_t> depo;
        for(size_t n=1;n<=7;++n)
        {
            depo.free();
            permutation perm(n);
            for( perm.boot(); perm.good(); perm.next() )
            {
                Y_ASSERT(depo.insert(perm));
            }
            if(n<=4)
            {
                const string fn = vformat("perm%u.dot", (unsigned)n );
                depo.get_root().graphViz(fn);
            }
        }
    }
    
    template <typename CODE, typename T>
    void testTree()
    {
        std::cerr << "-- tree<" << type_name_of<CODE>() << "," << type_name_of<T>() << ">" << std::endl;
       
         prefix_tree<CODE,T> tree;

        typedef vector<CODE> key_type;
        list<key_type>       keys;
        for(size_t cycle=1;cycle<=8;++cycle)
        {
            std::cerr << "insert: [";
            for(size_t iter=0;iter<64;++iter)
            {
                const size_t n = 1+alea.leq(4);
                key_type     key(n,as_capacity);
                for(size_t i=n;i>0;--i)
                {
                    key << alea.range<CODE>('a','d');
                }
                const T tmp = support::get<T>();
                if( tree.insert(key,tmp) )
                {
                    std::cerr << "+";
                    keys << key;
                }
                else
                {
                    std::cerr << "-";
                }
            }
            std::cerr << "]" << std::endl;

            std::cerr << "copy..." << std::endl;
            {
                prefix_tree<CODE,T> tree2( tree );
                Y_ASSERT(tree2.size()==tree.size());
                tree2.release();
                tree2 = tree;
                Y_ASSERT(tree2.size()==tree.size());
            }

            tree.sort( comparison::increasing<T> );

            std::cerr << "remove: [";

            alea.shuffle( *keys );
            while( keys.size() )
            {
                const key_type &key = keys.back();
                Y_ASSERT(tree.search(key));
                Y_ASSERT(tree.remove(key));
                keys.pop_back();
                Y_ASSERT(keys.size()==tree.size());
                std::cerr << "-";
            }

            std::cerr << "]" << std::endl;
        }
        
    }

}

Y_UTEST(prefix)
{
    
    dispText<char>();
    dispText<short>();
    dispText<int>();
    dispText<size_t>();

    disp<null_type>();
    disp<int>();

    _disp<uint8_t,null_type>(true);

    doStem<uint16_t,int>();
    doStem<uint32_t,unsigned>();

    std::cerr << std::endl;
    
    doDepot<int8_t>();
    doDepot<int16_t>();
    doDepot<int32_t>();
    doDepot<int64_t>();
    std::cerr << std::endl;
    
    testComb();
    std::cerr << std::endl;
    
    testPerm();
    std::cerr << std::endl;

    testTree<char,string>();
    std::cerr << std::endl;

#if 0
    {
        prefix_tree<char,int> tree;
        const string key1 = "hello";
        Y_CHECK(tree.insert(key1,1));
        const char *key2 = "world";
        Y_CHECK(tree.insert(key2,2));
        Y_CHECK(tree.insert("key",3));
        tree.get_root().graphViz("tree3.dot");

        Y_CHECK(tree.search(key1));
        Y_CHECK(tree.search(key2));
        Y_CHECK(tree.search("key"));
    }
#endif

    if(argc>1)
    {
        doStrings(argv[1]);
    }

}
Y_UTEST_DONE()


