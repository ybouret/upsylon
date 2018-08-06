#include "y/math/kernel/svd.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace math;

namespace
{
    
    template <typename T>
    static inline
    void do_test()
    {
        size_t pp = 0;
        for(size_t n=1;n<=32;++n)
        {
            for(size_t m=1;m<=32;++m)
            {
                matrix<T> a(n,m);
                vector<T> w(m);
                matrix<T> v(m,m);
                matrix<T> u(n,m);
                
                for(size_t iter=1;iter<=2;++iter)
                {
                   
                    for(size_t i=1;i<=n;++i)
                    {
                        for(size_t j=1;j<=m;++j)
                        {
                            a[i][j] = support::get<T>();
                        }
                    }
                    u=a;
                    if(!svd::build(u,w,v))
                    {
                        std::cerr << "unable to build svd" << std::endl;
                        continue;
                    }
                    
                    (std::cerr << ".").flush();
                    if( 0 == ((++pp) & 63) ) std::cerr << std::endl;
                }
            }
        }
        std::cerr << std::endl;
    }
    
}


Y_UTEST(svd)
{
    do_test<float>();
    
}
Y_UTEST_DONE()

