#include "y/sort/unique.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"

#include "support.hpp"

using namespace upsylon;


namespace
{
    template <typename T>
    void do_test()
    {
        for(size_t iter=0;iter<16;++iter)
        {
            const size_t n = 1 + alea.leq(100);
            vector<T> v(n, as_capacity );
            list<T>   l(n, as_capacity );

            for( size_t i=n;i>0;--i)
            {
                const T tmp = support::get<T>() % T(30);
                v.push_back(tmp);
                l.push_back(tmp);
            }

            std::cerr << "total=" << n << std::endl;
            std::cerr << "v=" << v << std::endl;
            std::cerr << "l=" << l << std::endl;

            unique(v);
            unique(l);
            std::cerr << "v=" << v << std::endl;
            std::cerr << "l=" << l << std::endl;

            Y_ASSERT(v.size()==l.size());
        }
    }
    
}

Y_UTEST(unique)
{
    do_test<unsigned short>();
    do_test<unsigned>();
}
Y_UTEST_DONE()

