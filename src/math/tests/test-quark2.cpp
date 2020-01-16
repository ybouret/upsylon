
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
        std::cerr << "<MUL " << typeid(T).name() << "," << typeid(U).name() << "," << typeid(V).name() << ">" << std::endl;

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

                        quark::mul(lhs, M, rhs);
                        if(loop)
                        {
                            quark::mul(tmp,M,rhs,*loop);
                            check1D(lhs,tmp);
                        }
                    }

                    {
                        support::fill1D(lhs);
                        support::fill1D(rhs);
                        support::fill2D(M);

                        quark::set(tmp,lhs);
                        quark::mul_add(lhs,M,rhs);
                        if(loop)
                        {
                            quark::mul_add(tmp,M,rhs,*loop);
                            check1D(lhs,tmp);
                        }
                    }

                    {
                        support::fill1D(lhs);
                        support::fill1D(rhs);
                        support::fill2D(M);

                        quark::set(tmp,lhs);
                        quark::mul_sub(lhs,M,rhs);
                        if(loop)
                        {
                            quark::mul_sub(tmp,M,rhs,*loop);
                            check1D(lhs,tmp);
                        }
                    }

                    {
                        support::fill1D(lhs);
                        support::fill1D(rhs);
                        support::fill2D(M);

                        quark::set(tmp,lhs);
                        quark::mul_subp(lhs,M,rhs);
                        if(loop)
                        {
                            quark::mul_subp(tmp,M,rhs,*loop);
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

                quark::mul(lhs,M,rhs);
                quark::set(cpy,lhs);
                quark::mul_sub(lhs,M,rhs);

                quark::ld(tmp,zt);
                check1D(tmp,lhs);
                quark::mul_add(lhs,M,rhs);
                check1D(cpy,lhs);
            }

        }
        std::cerr << std::endl;
    }

    template <typename T, typename U, typename V> static inline
    void doMMUL_TRN(concurrent::for_each *loop)
    {
        std::cerr << "<MUL_TRN " << typeid(T).name() << "," << typeid(U).name() << "," << typeid(V).name() << ">" << std::endl;

        const T zt = 0;
        const V zv = 0;
        for(size_t rows=1;rows<=32;++rows)
        {

            vector<V> rhs(rows,zv);
            for(size_t cols=1;cols<=32;++cols)
            {
                matrix<U> M(rows,cols);
                vector<T> lhs(cols,zt);
                vector<T> tmp(cols,zt);

                for(size_t iter=0;iter<4;++iter)
                {
                    {
                        support::fill1D(rhs);
                        support::fill2D(M);

                        quark::mul_trn(lhs, M, rhs);
                        if(loop)
                        {
                            quark::mul_trn(tmp,M,rhs,*loop);
                            check1D(lhs,tmp);
                        }
                    }


                    {
                        support::fill1D(lhs);
                        support::fill1D(rhs);
                        support::fill2D(M);

                        quark::set(tmp,lhs);
                        quark::mul_add_trn(lhs,M,rhs);
                        if(loop)
                        {
                            quark::mul_add_trn(tmp,M,rhs,*loop);
                            check1D(lhs,tmp);
                        }
                    }

                    {
                        support::fill1D(lhs);
                        support::fill1D(rhs);
                        support::fill2D(M);

                        quark::set(tmp,lhs);
                        quark::mul_sub_trn(lhs,M,rhs);
                        if(loop)
                        {
                            quark::mul_sub_trn(tmp,M,rhs,*loop);
                            check1D(lhs,tmp);
                        }
                    }

                    {
                        support::fill1D(lhs);
                        support::fill1D(rhs);
                        support::fill2D(M);

                        quark::set(tmp,lhs);
                        quark::mul_subp_trn(lhs,M,rhs);
                        if(loop)
                        {
                            quark::mul_subp_trn(tmp,M,rhs,*loop);
                            check1D(lhs,tmp);
                        }
                    }
                }
            }
        }
    }


    template <typename T> static inline
    void checkExactTRN()
    {
        std::cerr << "checkExactTRN<" << typeid(T).name() << ">" << std::endl;
        const T zt = 0;
        for(size_t rows=1;rows<=32;++rows)
        {
            std::cerr << ".";
            vector<T> rhs(rows,zt);


            for(size_t cols=1;cols<=32;++cols)
            {
                vector<T> lhs(cols,zt);
                vector<T> tmp(cols,zt);
                vector<T> cpy(cols,zt);
                matrix<T> M(rows,cols);
                support::fill1D(rhs);
                support::fill2D(M);

                quark::mul_trn(lhs,M,rhs);
                quark::set(cpy,lhs);
                quark::mul_sub_trn(lhs,M,rhs);

                quark::ld(tmp,zt);
                check1D(tmp,lhs);
                quark::mul_add_trn(lhs,M,rhs);
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

    doMMUL_TRN<double,double,double>( &loop );
    doMMUL_TRN<double,int,double>( &loop );


    checkExact<mpz>();
    checkExactTRN<mpz>();

}
Y_UTEST_DONE()
