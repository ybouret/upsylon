
//! \file
#ifndef Y_MKL_APK_INCLUDED
#define Y_MKL_APK_INCLUDED 1

#include "y/yap/rational.hpp"
#include "y/mkl/kernel/determinant.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{

    namespace mkl
    {
        //______________________________________________________________________
        //
        //
        //! arbitrary precision kernel
        //
        //______________________________________________________________________
        struct apk
        {
            static const char det_fcn[]; //!< "apk::determinant"
            static const char adj_fcn[]; //!< "apk::adjoint"

            //! integer type determinant
            /**
             - assuming that T is an integer type
             - will convert to rational
             */
            template <typename T> static inline
            apz determinant(const matrix<T> &a)
            {
                assert(a.rows>0);
                assert(a.is_square);
                const size_t   n=a.rows;
                matrix<apq>    b(n,n);
                b.assign(a);
                const apq d = __determinant(b);
                return d.to_integer(det_fcn);
            }

            //! compute adjoint for integer types
            /**
             internal computation is made with internal
             minors of rationals.
             */
            template <typename T> static inline
            void adjoint(matrix<apz>     &target,
                         const matrix<T> &source )
            {
                assert(target.rows==source.rows);
                assert(target.cols==source.cols);
                assert(source.is_square);
                const size_t n = source.rows;
                if(n<=1)
                {
                    target[1][1] = 1;
                }
                else
                {
                    const size_t   nm1 = n-1;
                    matrix<apq>    M(nm1,nm1);
                    for(size_t c=n;c>0;--c)
                    {
                        addressable<apz> &tc = target[c];
                        for(size_t r=n;r>0;--r)
                        {
                            M.minor_of(source,r,c);
                            const apz d = __determinant(M).to_integer(adj_fcn);
                            tc[r] = ( (r+c) & 1) ? -d:d;
                        }
                    }
                }
            }

            //! simplify num/den by the greatest gcd
            static void simplify(addressable<apz> &num, apz &den, apz *ratio);

            //! simplify full matrix
            static void simplify(matrix<apz> &num, apz &den, apz *ratio);

            //! simplify by row
            /**
             compute and simplify dividers for each rows
             */
            static void simplify(addressable<apz> &d, matrix<apz> &num, const apz &den);

            //! simplified adjoint of gram matrix
            /**
             \param A the simplified output adjoint matrix
             \param M the input integral matrix
             \return denominator value, adjoint=A/denominator
             */
            template <typename T> static inline
            apz adjoint_gram(matrix<apz> &A, const matrix<T> &M)
            {
                assert(A.rows==A.cols);
                assert(M.rows==A.rows);
                const size_t n = A.rows;
                matrix<apz>  G(n,n); tao::gram(G,M);  //std::cerr << "G=" << G << std::endl;
                apz          d = apk::determinant(G); //std::cerr << "d=" << d << std::endl;
                apk::adjoint(A,G);                    //std::cerr << "A=" << A << std::endl;
                simplify(A,d,NULL);
                return d;
            }

            //! convert matrix
            template <typename T> static inline
            void convert(matrix<T>          &target,
                         const matrix<apz>  &source,
                         const char         *when=0)
            {
                assert( target.same_size_than(source) );
                T         *tgt = *target;
                const apz *src = *source;
                for(size_t i=target.items;i>0;)
                {
                    --i;
                    tgt[i] = src[i].cast_to<T>(when);
                }

            }

            //! convert linear stuff
            template <typename T> static inline
            void convert(addressable<T> &target, const accessible<apz> &source, const char *when=0)
            {
                assert( target.size() <= source.size() );
                for(size_t i=target.size();i>0;--i)
                {
                    target[i] = source[i].cast_to<T>(when);
                }

            }

            //! check if rows of source are linearly independant
            template <typename T> static inline
            bool has_maximal_rank(const matrix<T> &source)
            {
                const size_t r = source.rows;
                matrix<apq>  G(r,r);
                tao::gram(G,source);
                return LU::build(G);
            }

            

            //! compute the rank of source rows
            static size_t gram_rank_of(const matrix<apz> &source);

            
            //! compute the rank of M with internal conversion
            template <typename T> static inline
            size_t rank(const matrix<T> &M)
            {
                if(M.rows<=M.cols)
                {
                    matrix<apz> source(M.rows,M.cols);
                    source.assign(M);
                    return gram_rank_of(source);
                }
                else
                {
                    matrix<apz> source(M.cols,M.rows);
                    source.assign_transpose(M);
                    return gram_rank_of(source);
                }
            }

            static const char complete_ortho_fn[]; //!< "complete_ortho"

            //! complete U with an orthogonal generator V
            /**
             - compute the projection matrix P on U
             - compute the projection matrix Q on U_ortho : Q=I-P
             - the rows of V are some rows of Q': try the different combination
             with smallest vectors first
             */
            static void complete_ortho_(const matrix<apz> &U, matrix<apz> &V);

            //! wrapper for integer types
            template <typename X, typename Y> static inline
            void complete_ortho(const matrix<X> &A, matrix<Y> &B)
            {
                matrix<apz> U(A.rows,A.cols); U.assign(A);
                matrix<apz> V(B.rows,B.cols);
                complete_ortho_(U,V);
                convert(B,V,complete_ortho_fn);
            }


        };
        
    }

}
#endif
