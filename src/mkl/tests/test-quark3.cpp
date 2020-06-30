
#include "y/mpl/rational.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/list.hpp"
#include "y/type/point3d.hpp"
#include "y/utest/run.hpp"
#include "support.hpp"
#include "y/memory/pooled.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;
using namespace mkl;

namespace {

    template <typename T>
    static inline void check(const char      *name,
                             const matrix<T> &lhs,
                             const matrix<T> &rhs )
    {
        for(size_t r=lhs.rows;r>0;--r)
        {
            for(size_t c=lhs.cols;c>0;--c)
            {
                if( __mod2( lhs[r][c] - rhs[r][c] ) > 0 )
                {
                    std::cerr << "[error for <" << type_name_of<T>() << "> '" << name << "']" << std::endl;
                    throw exception("%s",name);
                }
            }
        }
    }

    template <typename T> static inline
    void doMOD2(concurrent::for_each *loop, const size_t w=100)
    {
        std::cerr << "<MMOD2 " << type_name_of<T>() <<  ">" << std::endl;
        matrix<T> A(1+alea.leq(w),1+alea.leq(w));
        matrix<T> B(A.rows,A.cols);
        support::fill2D(A);
        B.assign(A);

        Y_ASSERT( __mod2( quark::mmod2<T>::of(A) - quark::mmod2<T>::of(B) ) <= 0 );
        Y_ASSERT( __mod2( quark::mmod2<T>::of(A,B) ) <= 0 );
        if( loop )
        {
            std::cerr << "approx: " << __mod2( quark::mmod2<T>::of(A) - quark::mmod2<T>::of(A,*loop) ) << std::endl;
            Y_ASSERT( __mod2( quark::mmod2<T>::of(A,B,*loop) ) <= 0 );
        }
    }


    template <typename T,typename U,typename V> static inline
    void doMMUL( concurrent::for_each *loop)
    {
        std::cerr << "<MMUL " << type_name_of<T>() << "," << type_name_of<U>() << "," << type_name_of<V>() << ">" << std::endl;

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
                            check("mmul",A,AA);
                        }

                        support::fill2D(A);
                        AA = A; check("=",A,AA);
                        quark::mmul_add(A,B,C);
                        if( loop )
                        {
                            quark::mmul_add(AA, B, C, *loop);
                            check("mmul_add",A,AA);
                        }

                        support::fill2D(A);
                        AA = A; check("=",A,AA);
                        quark::mmul_sub(A,B,C);
                        if( loop )
                        {
                            quark::mmul_sub(AA, B, C, *loop);
                            check("mmul_sub",A,AA);
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
                            check("mmul_rtrn",A,AA);
                        }

                        support::fill2D(A);
                        AA = A; check("=",A,AA);
                        quark::mmul_add_rtrn(A,B,C);
                        if( loop )
                        {
                            quark::mmul_add_rtrn(AA, B, C, *loop);
                            check("mmul_add_rtrn",A,AA);
                        }

                        support::fill2D(A);
                        AA = A; check("=",A,AA);
                        quark::mmul_sub_rtrn(A,B,C);
                        if( loop )
                        {
                            quark::mmul_sub_rtrn(AA, B, C, *loop);
                            check("mmul_sub_rtrn",A,AA);
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

    doMOD2<float> ( &loop );
    doMOD2<double>( &loop );
    doMOD2<mpz>   ( NULL  );
    doMOD2<mpq>   ( NULL, 10 );

    doMMUL<float,float,float>( &loop );
    doMMUL<double,int,int>   ( &loop );
    doMMUL<mpz,int,short>    ( NULL  );
}
Y_UTEST_DONE()

