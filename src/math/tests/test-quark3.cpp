
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

    template <typename T>
    static inline void check( const matrix<T> &lhs, const matrix<T> &rhs )
    {
        for(size_t r=lhs.rows;r>0;--r)
        {
            for(size_t c=lhs.cols;c>0;--c)
            {
                Y_ASSERT( __mod2( lhs[r][c] - rhs[r][c] ) <= 0 );
            }
        }
    }

    template <typename T,typename U,typename V> static inline
    void doMMUL( concurrent::for_each *loop)
    {
        std::cerr << "<MMUL " << typeid(T).name() << "," << typeid(U).name() << "," << typeid(V).name() << ">" << std::endl;

        for(size_t r=1;r<=16;r += 1+alea.leq(4))
        {
            std::cerr << "#rows=" << r << std::endl;
            for(size_t c=1;c<=16;c+=1+alea.leq(4))
            {
                std::cerr << "    #cols=" << c << "\t[";
                matrix<T> A(r,c);
                matrix<T> AA(r,c);

                for(size_t k=1;k<=24;k += 1+alea.leq(4) )
                {
                    (std::cerr << k << "/").flush();
                    {
                        matrix<U> B(r,k);
                        matrix<V> C(k,c);
                        support::fill2D(B);
                        support::fill2D(C);
                        quark::mmul(A, B, C);
                        if(loop)
                        {
                            quark::mmul(AA, B, C, *loop);
                            check(A,AA);
                        }

                        support::fill2D(A);
                        AA = A; check(A,AA);
                        quark::mmul_add(A,B,C);
                        if( loop )
                        {
                            quark::mmul_add(AA, B, C, *loop);
                            check(A,AA);
                        }

                        support::fill2D(A);
                        AA = A; check(A,AA);
                        quark::mmul_sub(A,B,C);
                        if( loop )
                        {
                            quark::mmul_sub(AA, B, C, *loop);
                            check(A,AA);
                        }
                    }

                    {
                        matrix<U> B(r,k);
                        matrix<V> C(c,k);
                        support::fill2D(B);
                        support::fill2D(C);
                        quark::mmul_rtrn(A, B, C);
                        if(loop)
                        {
                            quark::mmul_rtrn(AA, B, C, *loop);
                            check(A,AA);
                        }

                        support::fill2D(A);
                        AA = A; check(A,AA);
                        quark::mmul_add_rtrn(A,B,C);
                        if( loop )
                        {
                            quark::mmul_add_rtrn(AA, B, C, *loop);
                            check(A,AA);
                        }

                        support::fill2D(A);
                        AA = A; check(A,AA);
                        quark::mmul_sub_rtrn(A,B,C);
                        if( loop )
                        {
                            quark::mmul_sub_rtrn(AA, B, C, *loop);
                            check(A,AA);
                        }
                    }



                }
                std::cerr << "]" << std::endl;

            }
        }
    }
}

Y_UTEST(quark3)
{
    concurrent::simd loop;
    doMMUL<float,float,float>( &loop );
    doMMUL<double,int,int>( &loop );

    doMMUL<mpz,int,short>( NULL );
}
Y_UTEST_DONE()

