//! \file

#ifndef Y_MKL_SOLVE_ZIRCON_INCLUDED
#define Y_MKL_SOLVE_ZIRCON_INCLUDED 1

#include "y/mkl/kernel/eigen.hpp"
#include "y/mkl/kernel/lambda.hpp"
#include "y/sequence/arrays.hpp"
#include "y/mkl/tao.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/ios/ocstream.hpp"

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
        class zircon : public kernel::lambdas<T>
        {
        public:
            Y_DECL_ARGS(T,type);
            typedef arrays<type> arrays_type;
            typedef array<type>  array_type;
            typedef matrix<type> matrix_type;

            explicit zircon(const bool verbosity=false) :
            kernel::lambdas<T>(),
            N(0),
            _(10),
            F( _.next() ),
            X( _.next() ),
            Ftry( _.next() ),
            Xtry( _.next() ),
            G( _.next() ),
            U( _.next() ),
            V( _.next() ),
            S( _.next() ),
            Fsqr( _.next() ),
            step( _.next() ),
            J(),
            K(),
            H(),
            P(),
            verbose(verbosity)
            {
            }

            virtual ~zircon() throw()
            {
            }

            size_t      N;
            arrays_type _;
            array_type &F;       //!< function value
            array_type &X;       //!< current  position
            array_type &Ftry;    //!< trial F
            array_type &Xtry;    //!< trial X
            array_type &G;       //!< gradient of F^2/2:  K*F
            array_type &U;       //!< H eigenvalues
            array_type &V;       //!< H modified eigenvalues
            array_type &S;       //!< - transpose(P)*G
            array_type &Fsqr;    //!< to compute F^2/2
            array_type &step;    //!< estimated step

            matrix_type J;       //!< Jacobian
            matrix_type K;       //!< transpose(J)
            matrix_type H;       //!< K*J
            matrix_type P;       //!< H=P*U*transpose(P)
            int         p;       //!< for lambdas
            bool        verbose; //!< for verbosity

            template <
            typename FUNCTION,
            typename JACOBIAN>
            bool cycle(addressable<type> &x,
                       FUNCTION          &f,
                       JACOBIAN          &fjac,
                       int               &p,
                       const char        *trace= 0)
            {
                assert(p>=this->pmin);
                assert(p<=this->pmax);
                
                // init
                N = x.size();
                _  .acquire(N);
                J.make(N,N);
                K.make(N,N);
                H.make(N,N);
                P.make(N,N);

                call_g<FUNCTION> g = { f, X, step, Xtry, Ftry, Fsqr };

                // initialize
                tao::set(X,x);
                f(F,x);
                type   g0    = f2g(Fsqr,F);
                size_t count = 0;
                save(trace,X);
                while(true)
                {
                    // start from F(X)
                    ++count;
                    if(!topology(fjac))
                    {
                        Y_ZIRCON_PRINTLN("<singular>");
                        return false;
                    }


                    Y_ZIRCON_PRINTLN("F=" << F);
                    Y_ZIRCON_PRINTLN("g0 = " << g0 << "/" << g(0));
                    Y_ZIRCON_PRINTLN("J=" << J);
                    Y_ZIRCON_PRINTLN("G=" << G);
                    Y_ZIRCON_PRINTLN("H=" << H);
                    Y_ZIRCON_PRINTLN("U=" << U);
                    Y_ZIRCON_PRINTLN("P=" << P);
                    Y_ZIRCON_PRINTLN("S=" << S);

                    if(compute_step(p))
                    {
                        Y_ZIRCON_PRINTLN("step=" << step);

                    }

                    type g1 = g(1);
                    Y_ZIRCON_PRINTLN("g1 = " << g1);
                    while(g1>g0)
                    {
                        if(++p>this->pmax)
                        {
                            Y_ZIRCON_PRINTLN("<stuck>");
                            return false;
                        }
                        upgrade_step(p);
                        g1 = g(1);
                        Y_ZIRCON_PRINTLN("g1 = " << g1 << "@p=" << p );
                    }

                    g0 = g1;
                    for(size_t i=N;i>0;--i)
                    {
                        F[i] = Ftry[i];
                        X[i] = Xtry[i];
                    }
                    save(trace,X);


                    if(count>=12) break;

                }



                return true;
            }

            


        private:
            Y_DISABLE_COPY_AND_ASSIGN(zircon);

            static inline void save(const char *trace, const accessible<type> &v)
            {
                if(trace)
                {
                    ios::ocstream fp(trace,true);
                    fp("%.15g",v[1]);
                    for(size_t i=2;i<=v.size();++i)
                    {
                        fp(" %.15g", v[i]);
                    }
                    fp << '\n';
                }
            }

            static inline type f2g(addressable<type> &sq, const accessible<type> &v) throw()
            {
                static const_type half(0.5);
                const size_t      n = v.size();
                for(size_t i=n;i>0;--i)
                {
                    sq[i] = square_of(v[i]);
                }
                return half * sorted_sum(sq);
            }


            template <typename FUNC>
            struct call_g
            {
                FUNC                   &f;
                const accessible<type> &Xorg;
                const accessible<type> &step;
                addressable<type>      &Xtry;
                addressable<type>      &Ftry;
                addressable<type>      &Fsqr;
                inline type operator()(const_type u)
                {
                    tao::muladd(Xtry,Xorg,u,step);
                    f(Ftry,Xtry);
                    return f2g(Fsqr,Ftry);
                }
            };





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

            inline void upgrade_step(const int p)
            {
                assert(p>=this->pmin);
                assert(p<=this->pmax);
                const kernel::lambdas<T> &lam    = *this;
                array_type               &rhs    = Fsqr;
                const_type                lambda = lam[p];
                for(size_t i=N;i>0;--i)
                {
                    rhs[i] =  -S[i]/(U[i]+lambda);
                }
                tao::mul(step,P,rhs);
            }

            inline bool compute_step(int &p) throw()
            {
                const kernel::lambdas<T> &lam = *this;
                array_type               &rhs = Fsqr;
            TRY_STEP:
                const_type  lambda = lam[p];
                Y_ZIRCON_PRINTLN("step_from(p=" << p << ", lam=" << lambda << ")");
                for(size_t i=N;i>0;--i)
                {
                    V[i]   = U[i] + lambda;
                    if(V[i]<=numeric<mutable_type>::tiny)
                    {
                        if(++p>lam.pmax)
                        {
                            return false;
                        }
                        goto TRY_STEP;
                    }
                    rhs[i] = -S[i]/V[i];
                }
                tao::mul(step,P,rhs);
                return true;
            }

        };

    }
}

#endif
