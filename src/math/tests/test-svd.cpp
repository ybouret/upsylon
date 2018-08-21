#include "y/math/kernel/svd.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "support.hpp"
#include "y/math/utils.hpp"
#include "y/math/kernel/tao.hpp"

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
            vector<T> b(n); // A*x=b
            vector<T> x(n); // x values
            vector<T> y(n); // y=A*x
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
                    

                    
                    if(n==m)
                    {
                        const size_t ker = __find<T>::truncate(w);
                        if(ker>0)
                        {
                            std::cerr << ker;
                        }
                        
                        for(size_t i=1;i<=n;++i)
                        {
                            b[i]  = support::get<T>();
                        }
                        svd::solve(u,w,v,b,x);
                        tao::mul(y,a,x);
                        const T RMS = tao::rms(y,b);
                        std::cerr << '<' << RMS << '>';
                    }
                    
                    (std::cerr << ".").flush();
                    if( 0 == ((++pp) & 63) ) std::cerr << std::endl;
                }
            }
        }
        std::cerr << std::endl;

        for(size_t iter=1;iter<=32;++iter)
        {
            for(;;)
            {
                const size_t n = 1 + alea.leq(20);
                const size_t m = 1 + alea.leq(20);
                if(n==m) continue;
                matrix<T> P(n,m);
                for(size_t i=1;i<=n;++i)
                {
                    for(size_t j=1;j<=m;++j)
                    {
                        P[i][j] = support::get<T>();
                    }
                }
                matrix<T> Q;
                if(!svd::orthonormal(Q,P))
                {
                    std::cerr << "-";
                }
                else
                {
                    std::cerr << "+";
                }
                break;
            }
        }
        std::cerr << std::endl;
    }
}


Y_UTEST(svd)
{
    do_test<float>();
    do_test<double>();
}
Y_UTEST_DONE()

