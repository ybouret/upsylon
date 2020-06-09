#include "support.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/utest/run.hpp"
#include "y/sequence/vector.hpp"
#include "y/mkl/utils.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace mkl;

namespace
{
    template <typename T>
    static inline void do_test(const size_t nmax = 30)
    {
        std::cerr << "Using LU<" << type_name_of<T>() << ">" << std::endl;
        for(size_t n=1;n<=nmax;++n)
        {
            std::cerr << "\t\t" << n << "x" << n << std::endl;
            matrix<T> a(n,n);
            for(size_t i=n;i>0;--i)
            {
                for(size_t j=n;j>0;--j)
                {
                    a[i][j] = support::get<T>();
                }
            }

            const matrix<T> a0 = a;
            if( LU::build(a) )
            {
                std::cerr << "[SUCCESS]" << std::endl;
            }
            else
            {
                std::cerr << "[FAILURE]" << std::endl;
                continue;
            }
            std::cerr << "|_checking residue" << std::endl;
            vector<T> r(n);
            for(size_t i=n;i>0;--i)
            {
                r[i] = support::get<T>();
            }

            vector<T> u = r;
            LU::solve<T>(a,u);
            vector<T> d = r;
            quark::mul_sub(d,a0,u);
            std::cerr << "\t|d|^2=" << quark::mod2<T>::of(d) << std::endl;
        }
    }

    template <typename T>
    static inline void do_test_inv(const size_t nmax=5)
    {
        for(size_t n=1;n<=nmax;++n)
        {
            std::cerr << "inverse<" << type_name_of<T>() << "> " << n << "x" << n << std::endl;
            matrix<T> a(n,n);
            for(size_t i=n;i>0;--i)
            {
                for(size_t j=n;j>0;--j)
                {
                    a[i][j] = support::get<T>();
                }
            }
            const matrix<T> a0 = a;
            if( LU::build(a) )
            {
                std::cerr << "\t[LU SUCCESS]" << std::endl;
            }
            else
            {
                std::cerr << "\t[LU FAILURE]" << std::endl;
                continue;
            }
            matrix<T> b(n,n);
            LU::inverse(a,b);
            matrix<T> P(n,n);
            quark::mmul(P,a0,b);
            //__find<T>::truncate(P);
            std::cerr << "P=" << P << std::endl;

        }
    }
}

Y_UTEST(LU)
{
    do_test<float>();
    do_test<double>();
    do_test< complex<float>  >();
    do_test< complex<double> >();

    do_test<mpq>(5);

    do_test_inv<float>();
    do_test_inv<double>();
    do_test_inv< complex<float> >();
    do_test_inv< complex<double> >();
    do_test_inv<mpq>();

}
Y_UTEST_DONE()

