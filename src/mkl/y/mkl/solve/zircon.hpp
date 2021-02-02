//! \file

#ifndef Y_MKL_SOLVE_ZIRCON_INCLUDED
#define Y_MKL_SOLVE_ZIRCON_INCLUDED 1

#include "y/mkl/kernel/eigen.hpp"
#include "y/sequence/arrays.hpp"
#include "y/mkl/tao.hpp"
#include "y/sort/sorted-sum.hpp"

namespace upsylon
{
    namespace mkl
    {

        namespace kernel
        {
            struct zircon
            {
                static const char label[]; //!< "[zircon] "
            };
        }


#define Y_ZIRCON_PRINTLN(MSG) do { if(verbose) { std::cerr << kernel::zircon::label << MSG << std::endl; } } while(false)

        template <typename T>
        class zircon
        {
        public:
            Y_DECL_ARGS(T,type);
            typedef arrays<type> arrays_type;
            typedef array<type>  array_type;
            typedef matrix<type> matrix_type;

            explicit zircon() :
            N(0),
            _(8),
            F( _.next() ),
            X( _.next() ),
            G( _.next() ),
            U( _.next() ),
            V( _.next() ),
            S( _.next() ),
            Fsqr( _.next() ),
            step( _.next() ),
            J(),
            K(),
            H(),
            P()
            {
            }

            virtual ~zircon() throw()
            {
            }

            size_t      N;
            arrays_type _;
            array_type &F;    //!< function value
            array_type &X;    //!< current  position
            array_type &G;    //!< gradient of F^2/2:  K*F
            array_type &U;    //!< H eigenvalues
            array_type &V;    //!< H modified eigenvalues
            array_type &S;    //!< - transpose(P)*G
            array_type &Fsqr; //!< to compute F^2/2
            array_type &step; //!< estimated step
            matrix_type J;    //!< Jacobian
            matrix_type K;    //!< transpose(J)
            matrix_type H;    //!< K*J
            matrix_type P;    //!< H=P*U*transpose(P)

            bool        verbose;

            template <
            typename FUNCTION,
            typename JACOBIAN>
            bool cycle(addressable<type> &x, FUNCTION &f, JACOBIAN &fjac)
            {
                // init
                N = x.size();
                _  .acquire(N);
                J.make(N,N);
                K.make(N,N);
                H.make(N,N);
                P.make(N,N);

                // initialize
                tao::set(X,x);

                //
                f(F,x);
                if(!topology(fjac))
                {
                    Y_ZIRCON_PRINTLN("<singular>");
                    return false;
                }
                type g0 = f2g(F);


                Y_ZIRCON_PRINTLN("F=" << F);
                Y_ZIRCON_PRINTLN("g0 = " << g0);
                Y_ZIRCON_PRINTLN("J=" << J);
                Y_ZIRCON_PRINTLN("G=" << G);
                Y_ZIRCON_PRINTLN("H=" << H);
                Y_ZIRCON_PRINTLN("U=" << U);
                Y_ZIRCON_PRINTLN("P=" << P);
                Y_ZIRCON_PRINTLN("S=" << S);

                if(step_from(0))
                {
                    Y_ZIRCON_PRINTLN("step=" << step);

                }

                return true;
            }

            


        private:
            Y_DISABLE_COPY_AND_ASSIGN(zircon);

            inline type f2g(const accessible<type> &v)
            {
                static const_type half(0.5);
                for(size_t i=N;i>0;--i)
                {
                    Fsqr[i] = square_of(v[i]);
                }
                return sorted_sum(Fsqr)*half;
            }


            template <typename JACOBIAN> inline
            bool topology(JACOBIAN &fjac)
            {

                J.ld(0);                    // J = 0
                fjac(J,X);                  // user's J
                K.assign_transpose(J);      // K = transpose(J)
                tao::mul(G,K,F);            // G = transpose(J)*F
                tao::mmul(H,K,J);           // H = transpose(J)*J
                if( eigen::build(H,U,P) ) // H = P * U * transpose(P)
                {
                    tao::mul_trn(S,P,G);
                    return true;
                }
                else
                {
                    return false;
                }
            }

            inline bool step_from(const_type lam)
            {
                assert(lam>=0);
                array_type &rhs = Fsqr;
                for(size_t i=N;i>0;--i)
                {
                    V[i]   = U[i] + lam;
                    rhs[i] = -S[i]/V[i];
                }
                tao::mul(step,P,rhs);
                return true;
            }

        };

    }
}

#endif
