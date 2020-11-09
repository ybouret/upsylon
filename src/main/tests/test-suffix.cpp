
#include "y/suffix/key-to-path.hpp"
#include "y/suffix/strings.hpp"

#include "y/suffix/storage.hpp"


#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "y/ptr/auto.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "support.hpp"

#include "y/counting/comb.hpp"
#include "y/counting/perm.hpp"
#include "y/container/matrix.hpp"

#include "y/ios/icstream.hpp"

using namespace upsylon;
namespace
{
    template <typename CODE>
    void dispNode()
    {
        typedef suffix_node<CODE>  node_type;
        std::cerr << "-- suffix_node<" << type_name_of<CODE>() << "> \t: " << sizeof(node_type) << std::endl;
        auto_ptr<node_type>        root( new node_type(0,0,0) );
        for(size_t i=1+alea.leq(100);i>0;--i)
        {
            root->leaves.push_back( new node_type(0,CODE(i),0) );
            node_type *node = root->leaves.tail;
            if(alea.choice())
            {
               node->addr = suffix::in_use();
            }

            for(size_t j=1+alea.leq(10);j>0;--j)
            {
                node->leaves.push_back( new node_type(0,CODE(i),0) );
            }
        }
        typename node_type::pool_t pool;
        root->return_to(pool);
        root.dismiss();
    }

    template <typename CODE>
    void testTree()
    {
        const string &tid = type_name_of<CODE>();
        std::cerr << "-- suffix_tree<" << tid << ">" << std::endl;
        suffix_tree<CODE>    tree1;
        suffix_tree<CODE>    tree2;
        suffix_tree<CODE>    tree3;
        typedef vector<CODE> key_type;
        list<key_type> keys;
        void          *addr = suffix::in_use();
        std::cerr << "[";
        for(size_t iter=0;iter<32;++iter)
        {
            key_type key;
            for(size_t i=1+alea.leq(4);i>0;--i)
            {
                key << alea.range<CODE>('a','d');
            }
            if( tree1.insert_by( *key, key.size(), addr) )
            {
                keys << key;
                Y_ASSERT(tree2.insert_by(key,addr));
                Y_ASSERT(tree3.insert_at(key,addr));
                Y_ASSERT(tree1==tree2);
                Y_ASSERT(tree1==tree3);
                std::cerr << "+";
            }
            else
            {
                Y_ASSERT(!tree2.insert_by(key,addr));
                Y_ASSERT(!tree3.insert_at(key,addr));
                Y_ASSERT(tree1==tree2);
                Y_ASSERT(tree1==tree3);
                std::cerr << "-";
            }
            Y_ASSERT(keys.size()==tree1.root->frequency);
        }
        std::cerr << "]" << std::endl;
        Y_ASSERT(keys.size()==tree1.root->frequency);
        Y_ASSERT(keys.size()==tree2.root->frequency);
        {
            const string fileName = "tree-" + tid + ".dot";
            if(false)
            {
                tree1.root->graphViz(fileName);
            }
        }

        tree2.erase();
        tree2.clone(tree1);
        Y_ASSERT(tree1==tree2);

        alea.shuffle(*keys);
        std::cerr << "[";
        for( typename list<key_type>::iterator it=keys.begin();it!=keys.end();++it)
        {
            const key_type &key = *it;
            {
                const suffix_node<CODE> *node = tree1.find_by(*key,key.size());
                Y_ASSERT(node&&node->used);
            }
            {
                const suffix_node<CODE> *node = tree2.find_at(key);
                Y_ASSERT(node&&node->used);
            }

            {
                const suffix_node<CODE> *node = tree3.find_by(key);
                Y_ASSERT(node&&node->used);
            }

            std::cerr << "*" ;
        }
        std::cerr << "]" << std::endl;


        tree2.ditch();
        tree2.cache_load(1000);
        tree2.clone(tree1);
        Y_ASSERT(tree1==tree2);


        alea.shuffle(*keys);
        std::cerr << "[";
        for( typename list<key_type>::iterator it=keys.begin();it!=keys.end();++it)
        {
            const key_type &key = *it;
            Y_ASSERT( tree1.pull_by(*key,key.size() ) );
            Y_ASSERT( tree2.pull_by(key) );
            Y_ASSERT( tree3.pull_at(key) );

            std::cerr << "-";
        }
        std::cerr << "]" << std::endl;
    }
    
    template <typename T> static inline
    void testKnot()
    {
        typedef  suffix_knot<T>           knot_type;
        typedef typename  knot_type::list_type list_type;
        typedef typename  knot_type::pool_type pool_type;
        std::cerr << "-- suffix_knot<" << type_name_of<T>() << "> : " << sizeof(knot_type) << std::endl;
        
        list_type kl;
        pool_type kp;
        
        kp.cache( 1 + alea.leq(100) );
        std::cerr << "|_ #kl=" << kl.size << " #kp=" << kp.size << std::endl;
        for(size_t i=1+alea.leq(100);i>0;--i)
        {
            const T tmp = support::get<T>();
            kl.push_back( kp.query(tmp) );
        }
        std::cerr << "|_ #kl=" << kl.size << " #kp=" << kp.size << std::endl;
        kp.store(kl);
        std::cerr << "|_ #kl=" << kl.size << " #kp=" << kp.size << std::endl;
        for(size_t i=1+alea.leq(100);i>0;--i)
        {
            const T tmp = support::get<T>();
            kl.push_back( kp.query(tmp) );
        }
        std::cerr << "|_ #kl=" << kl.size << " #kp=" << kp.size << std::endl;
        
    }
    
    template <typename CODE, typename T> static inline
    void testGraph()
    {
        std::cerr << "-- suffix_graph<" << type_name_of<CODE>() << "," << type_name_of<T>()  << ">" << std::endl;

        typedef suffix_graph<CODE,T> graph_type;

        graph_type g1,g2,g3;
        
        typedef vector<CODE> key_type;
        list<key_type>       keys;
        
        std::cerr << "insert: [";
        for(size_t iter=0;iter<64;++iter)
        {
            key_type key;
            for(size_t i=1+alea.leq(4);i>0;--i)
            {
                key << alea.range<CODE>('a','d');
            }
            
            const T tmp = support::get<T>();
            if(g1.insert_by(*key, key.size(), tmp))
            {
                Y_ASSERT(g2.insert_by(key,tmp));
                Y_ASSERT(g3.insert_at(key,tmp));
                keys << key;
                std::cerr << "+";
            }
            else
            {
                std::cerr << "-";
            }
        }
        std::cerr << "]" << std::endl;
        
        std::cerr << "search: [";
        alea.shuffle(*keys);
        for(typename list<key_type>::iterator it=keys.begin();it!=keys.end();++it)
        {
            const key_type &key = *it;
            Y_ASSERT(g1.search_by(*key,key.size()));
            Y_ASSERT(g2.search_by(key));
            Y_ASSERT(g2.search_at(key));

            std::cerr << "*";
        }
        std::cerr << "]" << std::endl;

        {
            graph_type gg(g1);
            Y_ASSERT( gg.has_same_layout_than(g1) );
            gg.free();
            gg = g1;
            Y_ASSERT( g1.has_same_layout_than(gg) );

        }

        std::cerr << "remove: [";
        alea.shuffle(*keys);
        while(keys.size())
        {
            const key_type &key = keys.back();
            Y_ASSERT(g1.remove_by(*key,key.size()));
            Y_ASSERT(g2.remove_by(key));
            Y_ASSERT(g3.remove_at(key));

            Y_ASSERT(!g1.search_by(*key,key.size()));
            Y_ASSERT(!g2.search_by(key));
            Y_ASSERT(!g2.search_at(key));
            
            keys.pop_back();
            std::cerr << "-";
        }
        std::cerr << "]" << std::endl;

    }
 
    template <typename T> static inline
    void testGraphs()
    {
        testGraph<uint8_t, T>();
        testGraph<uint16_t,T>();
        testGraph<uint32_t,T>();
        testGraph<uint64_t,T>();
    }

    template <typename KEY>
    static inline void testKeys()
    {
        std::cerr << "-- key_to_path<" << type_name_of<KEY>() << ">" << std::endl;
        for(size_t iter=0;iter<8;++iter)
        {
            const KEY    tmp = support::get<KEY>();
            const char  *k   = key_to_path<KEY>::addr_of(tmp);
            const size_t n   = key_to_path<KEY>::size_of(tmp);
            std::cerr << tmp << " -> @" << (void*)k << " +" << n << std::endl;
        }
        std::cerr << std::endl;
    }


    typedef suffix_manifest<size_t> db_type;

    template <typename COUNTING>
    void testCouting(COUNTING &c, db_type &db1, db_type &db2)
    {
        db1.free();
        db2.free();
        for(c.boot();c.good();c.next())
        {
            Y_ASSERT(db1.insert_at(c));
            Y_ASSERT(db2.insert_by(&c[1],c.size()));
            Y_ASSERT(db1==db2);
        }
        db2.free();
        {
            const db_type db3(db1);
            Y_ASSERT(db3==db1);
            db2 = db3;
        }
        Y_ASSERT(db2==db1);
        for(c.boot();c.good();c.next())
        {
            Y_ASSERT(db1.search_at(c));
            Y_ASSERT(db2.search_by(&c[1],c.size()));
        }

        matrix<size_t> frame(c.count,c.space);
        counting::fill_frame(frame,c);
        vector<size_t> indx(c.count,0);
        for(size_t i=1;i<=c.count;++i) indx[i] = i;

        alea.shuffle(*indx,indx.size());
        for(size_t i=indx.size();i>0;--i)
        {
            const size_t j = indx[i];
            Y_ASSERT(db1.remove_at(frame[j]));
            Y_ASSERT(db2.remove_by( *frame[j], frame[j].size() ));

            Y_ASSERT(db1==db2);
            Y_ASSERT(!db1.search_at(frame[j]));
            Y_ASSERT(!db2.search_by( *frame[j], frame[j].size() ));
        }
    }

    static inline void testManifest()
    {
        std::cerr << "-- suffix_manifest" << std::endl;
        {
            db_type db1;
            db_type db2;
            std::cerr << "\tcomb" << std::endl;
            for(size_t n=1;n<=12;++n)
            {
                for(size_t k=1;k<=n;++k)
                {
                    combination comb(n,k);
                    testCouting(comb,db1,db2);
                }
            }

            std::cerr << "\tperm" << std::endl;
            for(size_t n=1;n<=6;++n)
            {
                permutation perm(n);
                testCouting(perm,db1,db2);
            }
        }
        std::cerr << std::endl;

    }

    static inline void testStrings(const char *fn)
    {
        const string fileName = fn;
        std::cerr << "-- suffix_strings(" << fileName << ")" << std::endl;
        suffix_strings<> db1,db2,db3;
        size_t nr = 10000;
        db1.reserve(nr);
        db2.reserve(nr);
        db3.reserve(nr);
        std::cerr << "reading..." << std::endl;
        {
            ios::icstream fp(fileName);
            string        line;
            std::cerr << "[";
            int count = 0;
            while(fp.gets(line))
            {
                //(std::cerr << line << '/').flush();
                if(0==(count++%1000))
                {
                    std::cerr << ".";
                }
                if(db1.insert(line))
                {
                    Y_ASSERT(db2.insert(*line,line.size()));
                    Y_ASSERT(db3.insert(*line));
                }
            }
            Y_ASSERT(db1==db2);
            Y_ASSERT(db1==db3);
            std::cerr << "]" << std::endl;
        }
        std::cerr << std::endl;
    }

    
    template <typename T> static inline
    void testStorage() 
    {
        std::cerr << "-- prefix_storage<" << type_name_of<T>() << ">" << std::endl;
        suffix_storage<T,suffix_collection> a1,a2;
        suffix_storage<T,container>         b1,b2;

        list<string> keys;
        std::cerr << "[";
        for(size_t iter=0;iter<64;++iter)
        {
            string key;
            for(size_t i=1+alea.leq(4);i>0;--i)
            {
                key << alea.range<char>('a','d');
            }
            const T tmp = support::get<T>();
            if(a1.insert(key,tmp))
            {
                Y_ASSERT(a2.insert(*key,tmp));
                Y_ASSERT(b1.insert(key,tmp));
                Y_ASSERT(b2.insert(*key,tmp));
                std::cerr << "+";
            }
            else
            {
                std::cerr << "-";
            }
        }
        std::cerr << "]" << std::endl;
        Y_CHECK(a1.has_same_layout_than(a2));
        Y_CHECK(b1.has_same_layout_than(b2));

        a2.sort_with( comparison::increasing<T> );
        a1.sort_with( comparison::decreasing<T> );
        Y_CHECK(a1.has_same_layout_than(a2));

        std::cerr << std::endl;

    }


    
}


Y_UTEST(suffix)
{
    dispNode<uint8_t>();
    dispNode<uint16_t>();
    dispNode<uint32_t>();
    dispNode<uint64_t>();
    std::cerr << std::endl;

    testTree<uint8_t>();
    testTree<uint16_t>();
    testTree<uint32_t>();
    testTree<uint64_t>();
    std::cerr << std::endl;
    
    testKnot<uint8_t>();
    testKnot<uint16_t>();
    testKnot<uint32_t>();
    testKnot<uint64_t>();
    testKnot<string>();
    std::cerr << std::endl;

    testGraphs<int>();
    testGraphs<string>();

    testKeys<int>();
    testKeys<string>();

    testManifest();

    testStorage<unsigned>();
    
    if(argc>1)
    {
        testStrings(argv[1]);
    }


}
Y_UTEST_DONE()


