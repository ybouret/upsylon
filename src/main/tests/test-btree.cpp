#include "y/associative/b-table.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/cblock.hpp"
#include "y/ptr/shared.hpp"
#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"

#include <typeinfo>
#include "y/ios/tools/graphviz.hpp"

using namespace upsylon;

namespace
{
    typedef memory::cblock_of<uint8_t> key_type;
    typedef shared_ptr<key_type>       shared_key;

    template <typename T>
    static inline void do_btree(bool save)
    {
        const string id = typeid(T).name();
        std::cerr << "btree<" << id << ">" << std::endl;
        std::cerr << "sizeof(node_type)=" << sizeof(typename btree<T>::node_type) << std::endl;
        btree<T>           tree;
        vector<shared_key> keys;

        std::cerr << "building..." << std::endl;
        for(size_t i=40+alea.leq(40);i>0;--i)
        {
            const T    tmp = support::get<T>();
            shared_key key = new key_type( alea.leq(3) );
            for(size_t j=0;j<key->size;++j)
            {
                key->data[j] = uint8_t( alea.leq(4) );
            }

            if( tree.insert_(key->data, key->size, tmp) )
            {
                keys.push_back(key);
            }
        }

        Y_CHECK( tree.entries() == keys.size() );
        std::cerr << "\t#tree.entries=" << tree.entries() << std::endl;

        if(save)
        {
            const string fn = "btree_" + id + ".dot";
            {
                ios::ocstream fp(fn);
                tree.graphviz(fp);
            }
            ios::GraphViz::Render(fn);
        }

        std::cerr << "searching..." << std::endl;
        alea.shuffle(*keys, keys.size() );
        for(size_t i=keys.size();i>0;--i)
        {
            const key_type &k = *keys[i];
            Y_ASSERT( tree.search_( k.data, k.size ) );
        }

        std::cerr << std::endl;


    }


    template <typename ITERATOR>
    void display_range( ITERATOR it, const ITERATOR end)
    {
        std::cerr << '{';
        for(;it!=end;++it)
        {
            std::cerr << ' ' << *it;
        }
        std::cerr << '}' << std::endl;
    }


}

Y_UTEST(btree)
{

    do_btree<unsigned short>(true);
    do_btree<string>(false);

    
    if( argc > 1 )
    {
        btree<unsigned>         tree;
        btable<string,unsigned> table;
        
        vector<string>  keys;
        {
            const string    fn = argv[1];
            ios::icstream   fp(fn);
            string          line;
            unsigned        count1=0;
            unsigned        count2=0;
            while( fp.gets(line) )
            {
                if(tree.insert_(line,count1))
                {
                    ++count1;
                    keys.push_back(line);
                }

                if(table.insert(line,count1))
                {
                    ++count2;
                }

                Y_ASSERT(count1==count2);
            }
        }
        std::cerr << "\t#tree.entries=" << tree.entries() << std::endl;

        alea.shuffle(*keys,keys.size());

        for(size_t i=keys.size();i>0;--i)
        {
            Y_ASSERT( tree.search_(keys[i]) );
        }
        display_range(table.begin(),table.end());
        display_range(table.rbegin(),table.rend());
        {
            const btable<string,unsigned> &cst = table;
            display_range(cst.begin(),  cst.end());
            display_range(cst.rbegin(), cst.rend());
        }

        if(keys.size()<=100)
        {
            const string fn = "btree.dot";
            {
                ios::ocstream fp(fn);
                tree.graphviz(fp);
            }
            ios::GraphViz::Render(fn);
        }
    }



}
Y_UTEST_DONE()


#include "y/associative/map.hpp"
#include "y/utest/timings.hpp"

namespace
{

    template <typename TABLE>
    void do_perf(const char *Kind,
                 TABLE &table,
                 const  size_t n,
                 const double tmx=1)
    {

        typedef typename TABLE::mutable_key_type key_t;

        table.free();
        vector<key_t> ok(n,as_capacity);
        vector<key_t> no(n,as_capacity);

        std::cerr << "Generating Table [[ " << Kind << " ]]/" << n;
        while( table.size() < n )
        {
            std::cerr.flush();
            const key_t  k  = support::get<key_t>();
            const size_t sz = alea.full<size_t>();
            if(!table.insert(k,sz))
            {
                continue;
            }
            ok.push_back_(k);
        } std::cerr << std::endl;

        Y_CHECK( table.size() == n );
        Y_CHECK( ok.size() == n );

        while( no.size() < n )
        {
            const key_t  k  = support::get<key_t>();
            if( table.search(k) ) continue;
            no.push_back_(k);
        }
        Y_CHECK( no.size() == n );

        double ok_speed = 0;
        Y_TIMINGS(ok_speed,tmx,
                  for(size_t i=n;i>0;--i)
                  {
                      if(!table.search( ok[i] )) throw exception("bad ok");
                  }
                  );
        std::cerr << "ok_speed=" << ok_speed << std::endl;

        double no_speed = 0;
        Y_TIMINGS(no_speed,tmx,
                  for(size_t i=n;i>0;--i)
                  {
                      if( table.search( no[i] )) throw exception("bad no");
                  }
                  );
        std::cerr << "no_speed=" << no_speed << std::endl;

    }


}

#include "y/string/convert.hpp"

Y_UTEST(assoc_perf)
{
    size_t n   = 1024;
    double tmx = 1.0;
    if(argc>1)
    {
        n = string_convert::to<size_t>(argv[1],"n");
    }

    if(argc>2)
    {
        tmx = string_convert::to<double>(argv[2],"tmx");
    }

    {
        btable<uint64_t,size_t> T(n,as_capacity);
        do_perf("B-Table",T,n,tmx);
    }
    
    {
        map<uint64_t,size_t> M(n,as_capacity);
        do_perf("H-Map",M,n,tmx);
    }



}
Y_UTEST_DONE()



