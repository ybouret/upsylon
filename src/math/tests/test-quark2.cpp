
#include "y/mpl/rational.hpp"
#include "y/math/kernel/quark.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/type/point3d.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/memory/pooled.hpp"
#include <typeinfo>

using namespace upsylon;
using namespace math;

namespace {

    template <typename LHS, typename RHS>
    void check1D( const LHS &lhs, const RHS &rhs )
    {
        for(size_t i=lhs.size();i>0;--i)
        {
            Y_ASSERT( __mod2(lhs[i]-rhs[i]) <= 0 );
        }
    }


    template <typename T, typename U, typename V>
    void doMMUL(concurrent::for_each *loop)
    {
        std::cerr << "<MMULs " << typeid(T).name() << "," << typeid(U).name() << "," << typeid(V).name() << ">" << std::endl;

        const T zt = 0;
        const V zv = 0;
        for(size_t rows=1;rows<=32;++rows)
        {
            vector<T> lhs(rows,zt);
            vector<T> tmp(rows,zt);
            for(size_t cols=1;cols<=32;++cols)
            {
                matrix<U> M(rows,cols);
                vector<V> rhs(cols,zv);

                for(size_t iter=0;iter<4;++iter)
                {
                    {
                        support::fill1D(rhs);
                        support::fill2D(M);

                        quark::mmul(lhs, M, rhs);
                        if(loop)
                        {
                            quark::mmul(tmp,M,rhs,*loop);
                            check1D(lhs,tmp);
                        }
                    }

                    {
                        support::fill1D(lhs);
                        support::fill1D(rhs);
                        support::fill2D(M);

                        quark::set(tmp,lhs);
                        quark::mmul_add(lhs,M,rhs);
                        if(loop)
                        {
                            quark::mmul_add(tmp,M,rhs,*loop);
                            check1D(lhs,tmp);
                        }
                    }

                    {
                        support::fill1D(lhs);
                        support::fill1D(rhs);
                        support::fill2D(M);

                        quark::set(tmp,lhs);
                        quark::mmul_sub(lhs,M,rhs);
                        if(loop)
                        {
                            quark::mmul_sub(tmp,M,rhs,*loop);
                            check1D(lhs,tmp);
                        }
                    }

                }

            }
        }

    }

    template <typename T> static inline
    void checkExact()
    {
        std::cerr << "checkExact<" << typeid(T).name() << ">" << std::endl;
        const T zt = 0;
        for(size_t rows=1;rows<=32;++rows)
        {
            std::cerr << ".";
            vector<T> lhs(rows,zt);
            vector<T> tmp(rows,zt);
            vector<T> cpy(rows,zt);
            for(size_t cols=1;cols<=32;++cols)
            {
                vector<T> rhs(cols,zt);
                matrix<T> M(rows,cols);
                support::fill1D(rhs);
                support::fill2D(M);

                quark::mmul(lhs,M,rhs);
                quark::set(cpy,lhs);
                quark::mmul_sub(lhs,M,rhs);

                quark::ld(tmp,zt);
                check1D(tmp,lhs);
                quark::mmul_add(lhs,M,rhs);
                check1D(cpy,lhs);
            }

        }
        std::cerr << std::endl;
    }


}

Y_UTEST(quark2)
{
    concurrent::simd loop;
    doMMUL<float,float,float>( &loop );
    doMMUL<float,unit_t,short>( &loop );


    checkExact<mpz>();

}
Y_UTEST_DONE()
