
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
                const T &L = lhs[r][c];
                const T &R = rhs[r][c];
                const T  delta = L-R;
                if( __mod2(delta) > 0 )
                {
                    std::cerr << "[error for <" << type_name_of<T>() << "> '" << name << "']" << std::endl;
                    std::cerr << "L=" << L << "(" << binary(L) << "), R=" << R << "(" << binary(R) << ")" << ", delta=" << delta << "(" << binary(delta) << ")" << std::endl;
                    throw exception("%s",name);
                }
            }
        }
    }

    template <typename T> static inline
    void doMOD2(concurrent::for_each *loop, const size_t w=100)
    {
        std::cerr << "<MMOD2 " << type_name_of<T>() <<  ">" << std::endl;
        typedef typename quark::mmod2<T>::real_type real_type;
        std::cerr << "\treal_type=" << type_name_of<real_type>() << std::endl;

        const size_t iter = (loop?1024:1);
        for(size_t it=0;it<iter;++it)
        {
            matrix<T> A(1+alea.leq(w),1+alea.leq(w));
            matrix<T> B(A.rows,A.cols);
            support::fill2D(A);
            B.assign(A);
            std::cerr << "\t[" << A.rows << "x" << A.cols << "]" << std::endl;
            for(size_t r=1;r<=A.rows;++r)
            {
                for(size_t c=1;c<=A.cols;++c)
                {
                    Y_ASSERT( __mod2(A[r][c]-B[r][c]) <= 0 );
                }
            }
            const real_type mod2A = quark::mmod2<T>::of(A);
            const real_type mod2B = quark::mmod2<T>::of(B);
            if(loop)
            {
                std::cerr << "mod2Aseq = " << mod2A << "("<< binary(mod2A) << ")" << std::endl;
                std::cerr << "mod2Bseq = " << mod2B << "("<< binary(mod2B) << ")" << std::endl;
            }

            Y_ASSERT( fabs_of( mod2A - mod2B ) <= 0 );
            Y_ASSERT( __mod2( quark::mmod2<T>::of(A,B) ) <= 0 );
            if( loop )
            {
                // summation is not in order guaranteed
                const real_type mod2A_par = quark::mmod2<T>::of(A,*loop);
                std::cerr << "mod2Apar = " << mod2A_par << "("<< binary(mod2A_par) << ")" << std::endl;
                std::cerr << "approx: " << fabs_of( mod2A - mod2A_par ) << std::endl;

                // term wise should be 0
                Y_ASSERT( fabs_of( quark::mmod2<T>::of(A,B,*loop) ) <= 0 );
            }
        }
    }


    template <typename T,typename U,typename V> static inline
    void doMMUL( concurrent::for_each *loop)
    {
        std::cerr << "<MMUL " << type_name_of<T>() << "," << type_name_of<U>() << "," << type_name_of<V>() << ">" << std::endl;

        for(size_t iter=0;iter<2;++iter)
        {
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

#if 0
                            support::fill2D(A);
                            AA = A; check("=",A,AA);
                            quark::mmul_sub_rtrn(A,B,C);
                            if( loop )
                            {
                                quark::mmul_sub_rtrn(AA, B, C, *loop);
                                check("mmul_sub_rtrn",A,AA);
                            }
#endif
                        }



                    }
                    std::cerr << "]" << std::endl;
                }
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
    doMOD2<mpq>   ( NULL, 8 );

    doMMUL<double,double,double>( &loop );
    doMMUL<float,float,float>( &loop );
    doMMUL<double,int,int>   ( &loop );
    doMMUL<mpz,int,short>    ( NULL  );
}
Y_UTEST_DONE()

