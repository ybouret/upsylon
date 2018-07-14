#include "y/associative/set.hpp"
#include "y/utest/run.hpp"

#include "support.hpp"

using namespace upsylon;

namespace
{
    template <typename KEY,typename T>
    class dummy
    {
    public:
        const KEY    label;
        const T      value;

        const KEY & key() const throw() { return label; }

        dummy( const KEY &l, const T &v ) : label( l ), value( v ) {}
        ~dummy() throw() {}

        dummy( const dummy &other ) : label(other.label), value(other.value) {}



    private:
        Y_DISABLE_ASSIGN(dummy);
    };

    template <typename KEY,typename T>
    static inline void do_test()
    {
        typedef set< KEY, dummy<KEY,T> > set_type;
        { set_type db0; }
        { set_type db1(100,as_capacity); }
        set_type db;
        size_t count = 0;
        for(size_t i=0;i<1000;++i)
        {
            const KEY k = support::get<KEY>();
            const T   v = support::get<T>();
            const dummy<KEY,T> d(k,v);

            if( db.insert(d) )
            {
                ++count;
            }
        }
        std::cerr << "count=" << count << std::endl;
        Y_CHECK(count==db.size());

    }

}

Y_UTEST(set)
{
    do_test<int,int>();
    do_test<string,int>();
    do_test<int,string>();
    do_test<string,string>();

}
Y_UTEST_DONE()

