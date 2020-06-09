#include "y/mkl/kernel/diagonalize.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/sequence/vector.hpp"

using namespace upsylon;
using namespace math;

namespace {
 
    template <typename T>
    static inline
    void do_test()
    {
        for(size_t n=32;n>=1;--n)
        {
            matrix<T> a(n,n);
            matrix<T> a0(n,n);
            
            vector<T> wr(n),wi(n);
            vector<T> y(n);
            
            size_t    nr=0;
            for(size_t i=1;i<=n;++i)
            {
                for(size_t j=1;j<=n;++j)
                {
                    a[i][j] = support::get<T>();
                }
            }
            a0.assign(a);
            //std::cerr << "a=" << a << std::endl;
            if( diagonalize::eig(a,wr,wi,nr) )
            {
                // std::cerr << "wr=" << wr << std::endl;
                // std::cerr << "wi=" << wi << std::endl;
                std::cerr << "#nr=" << nr << "/" << n << std::endl;
                matrix<T> ev(nr,n);
                diagonalize::eigv(ev,a0,wr);
                for(size_t i=1;i<=nr;++i)
                {
                    std::cerr << "\t" << wr[i] << " => " << ev[i] << std::endl;
                    const T              lam = wr[i];
                    const accessible<T> &v   = ev[i];
                    quark::mul(y,a0,v);
                    T rms = 0;
                    for(size_t i=n;i>0;--i)
                    {
                        rms += square_of(y[i]-lam*v[i]);
                    }
                    rms = sqrt_of( rms/n );
                    std::cerr << "\t\trms=" << rms << std::endl;
                }
            }
            else
            {
                std::cerr << "couldn't diagonalize" << std::endl;
            }
        }
    }
}

Y_UTEST(diag)
{
    do_test<float>();
    do_test<double>();
}
Y_UTEST_DONE()

