#include "y/associative/map.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"

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
        size_t count = 0;
        for(size_t i=0;i<1000;++i)
        {
            const KEY k = support::get<KEY>();
            const T   v = support::get<T>();
            if(db.insert(k,v))
            {
                ++count;
            }
        }
        std::cerr << "count=" << count << std::endl;
        Y_CHECK(count==db.size());
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

