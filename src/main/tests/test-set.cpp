#include "y/associative/set.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/ptr/intr.hpp"
#include "y/ptr/counted.hpp"
#include "support.hpp"

using namespace upsylon;

namespace
{
    template <typename KEY,typename T>
    class dummy : public counted_object
    {
    public:
        typedef intr_ptr< KEY,dummy<KEY,T> > pointer;
        const KEY    label;
        const T      value;

        const KEY & key() const throw() { return label; }

        dummy( const KEY &l, const T &v ) : counted_object(), label( l ), value( v ) {}
        virtual ~dummy() throw() {}
        dummy( const dummy &other ) : counted_object(), label(other.label), value(other.value) {}

        inline friend std::ostream & operator<<( std::ostream &os, const dummy &d)
        {
            os << "[" << d.label << ":" << d.value << "]";
            return os;
        }

    private:
        Y_DISABLE_ASSIGN(dummy);
    };

    template <typename KEY,typename T>
    static inline void do_test()
    {
        // by copy
        std::cerr << "-- by copy" << std::endl;
        {
            typedef set< KEY, dummy<KEY,T> > set_type;
            { set_type db0; }
            { set_type db1(100,as_capacity); }
            set_type db;
            size_t count = 0;
            vector<KEY> keys;
            for(size_t i=0;i<1000;++i)
            {
                const KEY k = support::get<KEY>();
                const T   v = support::get<T>();
                const dummy<KEY,T> d(k,v);

                if( db.insert(d) )
                {
                    ++count;
                    keys.push_back(k);
                }
            }
            std::cerr << "count=" << count << std::endl;
            Y_CHECK(count==db.size());
            Y_CHECK(count==keys.size());
            alea.shuffle(*keys,keys.size());
            const set_type &dbc = db;
            for(size_t i=1;i<=keys.size();++i)
            {
                Y_ASSERT(db.search(keys[i]));
                Y_ASSERT(dbc.search(keys[i]));
            }

            std::cerr << "iterators..." << std::endl;
            for( typename set_type::iterator i=db.begin();i!=db.end();++i)
            {
                std::cerr << i.key() << ":" << *i << std::endl;
            }


            for( typename set_type::const_iterator i=dbc.begin();i!=dbc.end();++i)
            {
                std::cerr << i.key() << ":" << *i << std::endl;
            }
#if 0
            std::cerr << "sorting by data" << std::endl;
            db.sort_data(comparison::increasing<T>);
            for( typename set_type::iterator i=db.begin();i!=db.end();++i)
            {
                std::cerr << i.key() << ":" << *i << std::endl;
            }
#endif
            
            std::cerr << "sorting by keys" << std::endl;
            db.sort_keys(comparison::increasing<KEY>);
            for( typename set_type::iterator i=db.begin();i!=db.end();++i)
            {
                std::cerr << i.key() << ":" << *i << std::endl;
            }


            for(size_t i=1;i<=keys.size();++i)
            {
                Y_ASSERT(db.remove(keys[i]));
            }
        }

        // by pointer
        std::cerr << "-- by pointer" << std::endl;
        {
            typedef typename dummy<KEY,T>::pointer dptr_t;
            typedef set<KEY,dptr_t>                dset_t;

            dset_t db;
            size_t count = 0;
            vector<KEY> keys;
            for(size_t i=0;i<1000;++i)
            {
                const KEY k = support::get<KEY>();
                const T   v = support::get<T>();
                const dptr_t d( new dummy<KEY,T>(k,v) );

                if( db.insert(d) )
                {
                    ++count;
                    keys.push_back(k);
                }
            }
            std::cerr << "count=" << count << std::endl;
            Y_CHECK(count==db.size());
            Y_CHECK(count==keys.size());
        }

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

