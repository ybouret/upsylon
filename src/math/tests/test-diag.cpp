#include "y/math/kernel/diagonalize.hpp"
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
        for(size_t n=1;n<=32;++n)
        {
            matrix<T> a(n,n);
            matrix<T> a0(n,n);
            
            vector<T> wr(n),wi(n);
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
                std::cerr << "wr=" << wr << std::endl;
                std::cerr << "wi=" << wi << std::endl;
                matrix<T> ev(nr,n);
                diagonalize::eigv(ev,a0,wr);
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
}
Y_UTEST_DONE()

