#include "y/associative/map.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;

namespace
{

    template <typename KEY,typename T>
    static inline void do_test()
    {
        typedef map<KEY,T> map_t;
        { map_t db0; }
        { map_t db1(5,as_capacity); }
        map_t db;
        vector<KEY> keys;
        size_t count = 0;
        for(size_t i=0;i<1000;++i)
        {
            const KEY k = support::get<KEY>();
            const T   v = support::get<T>();
            if(db.insert(k,v))
            {
                ++count;
                keys.push_back(k);
            }
            
        }
        std::cerr << "count=" << count << std::endl;
        Y_CHECK(count==db.size());
        Y_CHECK(count==keys.size());
        alea.shuffle(*keys,keys.size());
        const map_t &dbc = db;
        for(size_t i=1;i<=keys.size();++i)
        {
            Y_ASSERT(db.search(keys[i]));
            Y_ASSERT(dbc.search(keys[i]));
        }
        std::cerr << "iterators..." << std::endl;
        for( typename map_t::iterator i=db.begin();i!=db.end();++i)
        {
            std::cerr << i.key() << ":" << *i << std::endl;
        }

        {
            const map_t &cdb = db;
            for( typename map_t::const_iterator i=cdb.begin();i!=cdb.end();++i)
            {
                std::cerr << i.key() << ":" << *i << std::endl;
            }

        }
        for(size_t i=1;i<=keys.size();++i)
        {
            Y_ASSERT(db.remove(keys[i]));
        }
    }
    

}

Y_UTEST(map)
{
    map<size_t,int> m;
    for(size_t i=0;i<=1000;++i)
    {
        Y_ASSERT(m.insert(i, alea.full<int>() ));
    }
    Y_ASSERT(m.size()==1001);

    do_test<int,int>();
    do_test<string,int>();
    do_test<int,string>();
    do_test<string,string>();
}
Y_UTEST_DONE()

