
#include "y/sequence/crates.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/memory/allocator/pooled.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/type/rtti.hpp"

using namespace upsylon;

namespace
{
    template <typename T, typename ALLOCATOR>
    static inline void do_crates()
    {
        const size_t        n = alea.leq(80);
        crates<T,ALLOCATOR> data(n);
        Y_CHECK(data.capacity()==n);

        while( data.size() < n )
        {
            const T tmp = support::get<T>();
            if( alea.choice() )
            {
                std::cerr << '<';
                data.push_back(tmp);
            }
            else
            {
                std::cerr << '>';
                data.push_front(tmp);
            }
        }
        alea.shuffle(*data, data.size() );
        std::cerr << std::endl;

    }

    template <typename T>
    static inline void test_crates()
    {
        std::cerr << rtti::name_of<T>() << std::endl;
        do_crates<T,memory::global>();
        do_crates<T,memory::pooled>();
        do_crates<T,memory::dyadic>();
        std::cerr << std::endl;
    }
}

Y_UTEST(crates)
{
    test_crates<short>();
    test_crates<double>();
    test_crates<string>();

    {
        const size_t n = 2+alea.leq(30);
        crates<int> data(n);
        data.adjust(n,0);
        for(size_t i=n;i>0;--i)
        {
            data[i] = alea.partial<int>(8);
        }
        std::cerr << data << std::endl;
        alea.shuffle(*data,n);
        std::cerr << data << std::endl;
    }

}
Y_UTEST_DONE()

