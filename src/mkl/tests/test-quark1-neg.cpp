
#include "y/mkl/kernel/quark.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/type/point3d.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace mkl;

namespace {

    template <typename ARRAY>
    void load( ARRAY &arr  )
    {
        support::fill1D(arr);
    }

    template <typename LHS, typename RHS>
    void check1D( const LHS &lhs, const RHS &rhs )
    {
        for(size_t i=lhs.size();i>0;--i)
        {
            Y_ASSERT( mod2_of(lhs[i]-rhs[i]) <= 0 );
        }
    }

    template <typename T, typename U>
    static inline
    void doNEG( concurrent::for_each *loop )
    {
        std::cerr << "<NEG " << type_name_of<T>() << "," << type_name_of<U>()<< ">" << std::endl;

        const T zt = 0;
        const U zu = 0;
        for(size_t n=1;n<=1024*128;n <<= 1)
        {
            vector<T> t(n,zt), tt(n,zt);
            vector<U> u(n,zu);
            support::fill1D(u);

            quark::neg(t,u);
            if(loop)
            {
                quark::neg(tt,u,*loop);
                check1D(tt,t);
            }
            quark::neg(t);
            if(loop)
            {
                quark::neg(tt,*loop);
                check1D(tt,t);
            }
        }
    }

    template <typename T>
    static inline void checkExact()
    {
        std::cerr << "CheckExactNeg<" << type_name_of<T>()  << ">" << std::endl;
        const T zt = 0;
        for(size_t n=1;n<=1024;n <<= 1)
        {
            vector<T> a(n,zt);
            vector<T> b(n,zt);
            vector<T> c(n,zt);

            support::fill1D(a);
            quark::neg(b,a);
            for(size_t i=n;i>0;--i) { Y_ASSERT(b[i]==-a[i]); }
            quark::neg(c,b);
            check1D(a,c);
        }
    }

}

Y_UTEST(quark1_neg)
{
    concurrent::simd loop;

    doNEG<unit_t,int>( &loop );
    doNEG<double,float>( &loop );
    doNEG<apz,int>( NULL );
    checkExact<unit_t>();
    checkExact<apz>();
    checkExact<apq>();


}
Y_UTEST_DONE()
