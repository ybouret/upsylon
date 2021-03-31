//! \file

#ifndef Y_MKL_SOLVE_ZIRCON_INCLUDED
#define Y_MKL_SOLVE_ZIRCON_INCLUDED 1

#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/kernel/lambda.hpp"
#include "y/sequence/arrays.hpp"
#include "y/mkl/tao.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/ios/ocstream.hpp"

namespace upsylon
{
    namespace mkl
    {

        //______________________________________________________________________
        //
        //
        //! zircon common stuff
        //
        //______________________________________________________________________
        class zircon
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________

            //! cycle result
            enum status
            {
                success,  //!< numeric 0
                stalled,  //!< numeric convergence
                singular, //!< singular jacobian
                spurious  //!< not singular but no decreasing
            };
            static const char  label[]; //!< "[zircon] "
            static const char *readable(const status) throw(); //!< human readable status

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~zircon() throw(); //!< cleanup

        protected:
            explicit zircon() throw(); //!< setup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(zircon);
        };




        //! display info
#define Y_ZIRCON_PRINTLN(MSG) do { if(verbose) { std::cerr << label << MSG << std::endl; } } while(false)

        //______________________________________________________________________
        //
        //
        //! Zircon Is Repetitive Controled Operating Newton
        //
        //______________________________________________________________________
        template <typename T>
        class Zircon : public zircon, public kernel::lambdas<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);                //!< aliases
            typedef arrays<type> arrays_type;   //!< alias
            typedef array<type>  array_type;    //!< alias
            typedef matrix<type> matrix_type;   //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! initialize
            explicit Zircon(const bool verbosity=false) :
            kernel::lambdas<T>(),
            N(0),
            _(7),
            F(    _.next() ),
            X(    _.next() ),
            Ftry( _.next() ),
            Xtry( _.next() ),
            G(    _.next() ),
            temp( _.next() ),
            step( _.next() ),
            J(),
            K(),
            H(),
            C(),
            verbose(verbosity)
            {
            }

            //! cleanup
            virtual ~Zircon() throw() {}

            //__________________________________________________________________
            //
            //! cycle function
            //__________________________________________________________________
            template < typename FUNCTION, typename JACOBIAN>
            status cycle(addressable<type> &x,
                         FUNCTION          &f,
                         JACOBIAN          &fjac,
                         int               &p,
                         type              &g0,
                         const char        *trace=0)
            {
                p  = clamp<int>(this->pmin,p,this->pmax);
                g0 = -1;
                
                //--------------------------------------------------------------
                //
                // initialize memory
                //
                //--------------------------------------------------------------
                N = x.size();
                _.acquire(N);
                J.make(N,N);
                K.make(N,N);
                H.make(N,N);
                C.make(N,N);
                call_g<FUNCTION> g = { f, X, step, Xtry, Ftry, temp };

                //--------------------------------------------------------------
                //
                // initialize value
                //
                //--------------------------------------------------------------
                tao::set(X,x);
                f(F,X);
                g0 = f2g(temp,F);
                size_t count = 0;
                save(trace,X);

                //--------------------------------------------------------------
                //
                // Loop...
                //
                //--------------------------------------------------------------
                while(true)
                {

                    //----------------------------------------------------------
                    //
                    // start from F(X) => compute topology
                    //
                    //----------------------------------------------------------
                    ++count;
                    topology(fjac);

                    Y_ZIRCON_PRINTLN("---- cycle=" << count);
                    Y_ZIRCON_PRINTLN("F=" << F);
                    Y_ZIRCON_PRINTLN("g0 = " << g0 << "/" << g(0));
                    Y_ZIRCON_PRINTLN("J=" << J);
                    Y_ZIRCON_PRINTLN("G=" << G);
                    Y_ZIRCON_PRINTLN("H=" << H);

                    //----------------------------------------------------------
                    //
                    // try to compute a valid step from there
                    //
                    //----------------------------------------------------------
                    if(!compute_step_with(p))
                    {
                        Y_ZIRCON_PRINTLN("<singular>");
                        return singular;
                    }
                    Y_ZIRCON_PRINTLN("step=" << step);

                    //----------------------------------------------------------
                    //
                    // step must decrease the value
                    //
                    //----------------------------------------------------------
                    type g1 = g(1);
                    Y_ZIRCON_PRINTLN("g1 = " << g1);
                    bool decreasing = true;
                    while(g1>g0)
                    {
                        decreasing=false;
                        if(!increase(p) && !compute_step_with(p))
                        {
                            Y_ZIRCON_PRINTLN("<spurious>");
                            return spurious;
                        }
                        g1 = g(1);
                        Y_ZIRCON_PRINTLN("g1 = " << g1 << "@p=" << p );
                    }
                    const_type sigma = - tao::dot<type>::of(step,G);
                    Y_ZIRCON_PRINTLN("step  = " << step);
                    Y_ZIRCON_PRINTLN("sigma = " << sigma);

                    {
                        if(1==count)
                        {
                            ios::ocstream fp("zircon.dat");
                            for(type u=0;u<=2;u+=type(0.01))
                            {
                                fp("%g %g %g\n", u, g(u), g0-sigma*u );
                            }
                            g1=g(1);
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // update
                    //
                    //----------------------------------------------------------
                    g0 = g1;
                    const bool converged = has_converged();
                    save(trace,X);

                    if(g0<=0)
                    {
                        tao::upload(x,X);
                        Y_ZIRCON_PRINTLN("<success>");
                        return success;
                    }

                    if(converged)
                    {
                        tao::upload(x,X);
                        Y_ZIRCON_PRINTLN("<converged>");
                        return stalled;
                    }

                    if(decreasing) decrease(p);
                }



            }


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            size_t      N;       //!< variables size
            arrays_type _;       //!< memory for linear arrays
            array_type &F;       //!< function value
            array_type &X;       //!< current  position
            array_type &Ftry;    //!< trial F
            array_type &Xtry;    //!< trial X
            array_type &G;       //!< gradient of F^2/2:  K*F
            array_type &temp;    //!< to compute auxiliary
            array_type &step;    //!< estimated step
            matrix_type J;       //!< Jacobian
            matrix_type K;       //!< transpose(J)
            matrix_type H;       //!< transpose(J)*J
            matrix_type C;       //!< copy of H, regularise
            bool        verbose; //!< for verbosity

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Zircon);

            //__________________________________________________________________
            //
            //! append coordinates to a file
            //__________________________________________________________________
            static inline void save(const char *trace, const accessible<type> &v)
            {
                const size_t n = v.size();
                if(trace&&n>0)
                {
                    ios::ocstream fp(trace,true);
                    fp("%.15g",v[1]);
                    for(size_t i=2;i<=n;++i)
                    {
                        fp(" %.15g", v[i]);
                    }
                    fp << '\n';
                }
            }

            //__________________________________________________________________
            //
            //! sq[i] = v[i]^2, sorted_sum(sq)/2
            //__________________________________________________________________
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

            //__________________________________________________________________
            //
            //! wrapper for objective function
            //__________________________________________________________________
            template <typename FUNC>
            struct call_g
            {
                FUNC                   &f;
                const accessible<type> &Xorg;
                const accessible<type> &step;
                addressable<type>      &Xtry;
                addressable<type>      &Ftry;
                addressable<type>      &temp;
                
                inline type operator()(const_type u)
                {
                    tao::muladd(Xtry,Xorg,u,step);
                    f(Ftry,Xtry);
                    return f2g(temp,Ftry);
                }
            };


            //__________________________________________________________________
            //
            //! compute jacobian and derived quantities
            //__________________________________________________________________
            template <typename JACOBIAN> inline
            void topology(JACOBIAN &fjac)
            {

                J.ld(0);                    // J = 0
                fjac(J,X);                  // user's J
                K.assign_transpose(J);      // K = transpose(J)
                tao::mul(G,K,F);            // G = transpose(J)*F
                tao::gram(H,K);             // H = transpose(J)*J
            }

            //__________________________________________________________________
            //
            //! compute step from curvature and parameter
            //__________________________________________________________________
            inline bool __build_step_with(const int p)
            {
                assert(p>=this->pmin);
                assert(p<=this->pmax);
                static const_type         one = type(1);
                const kernel::lambdas<T> &lam = *this;
                const_type                lambda = lam[p];
                const_type                factor = one+lambda;
                Y_ZIRCON_PRINTLN("step_from(p=" << p << ",lam=" << lambda << ")");
                for(size_t i=N;i>0;--i)
                {
                    array<type>       &C_i =  C[i];
                    const array<type> &H_i =  H[i];
                    for(size_t j=N;j>0;--j) C_i[j] = H_i[j];
                    C_i[i] *= factor;
                }
                if( !LU::build(C) )
                {
                    return false;
                }
                else
                {
                    tao::neg(step,G);
                    LU::solve(C,step);
                    return true;
                }
            }

            //__________________________________________________________________
            //
            //! increase parameter if possible
            //__________________________________________________________________
            inline bool increase(int &p) const throw()
            {
                static const int PMAX = this->pmax;
                return (++p>PMAX);
            }

            //__________________________________________________________________
            //
            //! decrease parameter
            //__________________________________________________________________
            inline void decrease(int &p) const throw()
            {
                static const int PMIN = this->pmin;
                --p;
                if(p<=PMIN) p=PMIN;
            }

            //__________________________________________________________________
            //
            //! best effort step
            //__________________________________________________________________
            inline bool compute_step_with(int &p) throw()
            {
            BUILD:
                if(!__build_step_with(p))
                {
                    if(!increase(p)) return false;
                    goto BUILD;
                }
                return true;
            }


            //__________________________________________________________________
            //
            //! update (Ftry,Xtry) -> (F,X) with convergence checking
            //__________________________________________________________________
            inline bool has_converged() throw()
            {
                static const_type xtol = numeric<type>::sqrt_ftol;
                size_t i=N;
                bool   converged = true;
                for(;i>0;--i)
                {
                    const_type Xold = X[i];
                    const_type Xnew = Xtry[i];
                    const_type dX   = fabs_of(Xnew-Xold);
                    F[i] = Ftry[i];
                    X[i] = Xnew;
                    if( dX >  xtol * max_of(fabs_of(Xold),fabs_of(Xnew)))
                    {
                        converged = false;
                        --i;
                        break;
                    }
                }

                for(;i>0;--i)
                {
                    F[i] = Ftry[i];
                    X[i] = Xtry[i];
                }

                return converged;
            }

        };

    }
}

#endif
