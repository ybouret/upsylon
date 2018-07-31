#include "y/math/kernel/diag-symm.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"

using namespace upsylon;

namespace
{
    template <typename T>
    static inline void do_test()
    {
        for(size_t n=1;n<=5;++n)
        {
            matrix<T> a(n,n);
            vector<T> d(n);
            matrix<T> v(n,n);
            
            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=i;j<=n;++j)
                {
                    a[i][j] = a[j][i] = support::get<T>();
                }
            }
            
            std::cerr << "a=" << a << std::endl;
            if( !math::diag_symm::build(a,d,v) )
            {
                std::cerr << "diag_symm failure" << std::endl;
            }
            std::cerr << "d=" << d << std::endl;
            std::cerr << "v=" << v << std::endl;
            
        }
    }
}

Y_UTEST(diag_symm)
{
    do_test<float>();
    
}
Y_UTEST_DONE()



