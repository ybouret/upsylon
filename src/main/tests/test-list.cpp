#include "y/sequence/list.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "support.hpp"

using namespace upsylon;

namespace {

    template <typename T>
    void testListOf()
    {
        std::cerr << "-- list<" << type_name_of<T>() << ">" << std::endl;
        {
            list<T> L;
        }
        for(size_t n=0;n<1000;++n)
        {
            list<T> L(n,as_capacity);
            Y_ASSERT(n==L.capacity());
        }

        for(size_t n=0;n<100;++n)
        {
            const T tmp = support::get<T>();
            list<T> L(n,tmp);
            Y_ASSERT(n==L.size());
            {
                list<T> L2(L);
                L = L2;
            }
        }

        for(size_t iter=0;iter<8;++iter)
        {
            const size_t n = 10 + alea.leq(100);
            list<T> L;
            for(size_t i=n;i>0;--i)
            {
                const T tmp = support::get<T>();
                if( alea.choice() ) L.push_back( tmp ); else L.push_front( tmp );
            }
            Y_ASSERT(n==L.size());
            Y_ASSERT(0==L.available());
            L.reverse();
            Y_ASSERT(n==L.size());
            Y_ASSERT(0==L.available());
            L.free();
            Y_ASSERT(0==L.size());
            Y_ASSERT(n==L.available());
            for(size_t i=n;i>0;--i)
            {
                const T tmp = support::get<T>();
                if( alea.choice() ) L.push_back_( tmp ); else L.push_front_( tmp );
            }
            while( L.size() )
            {
                if( alea.choice() ) L.pop_back(); else L.pop_front();
            }
        }





        std::cerr << std::endl << std::endl;

    }


}

Y_UTEST(list)
{
    testListOf<int>();
    testListOf<string>();
}
Y_UTEST_DONE()


