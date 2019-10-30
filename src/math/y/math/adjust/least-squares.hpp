//! \file

#ifndef Y_ADJUST_LEAST_SQUARES_INCLUDED
#define Y_ADJUST_LEAST_SQUARES_INCLUDED 1

#include "y/math/adjust/context.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/oxide/field1d.hpp"
#include "y/ios/ocstream.hpp"
#include "y/math/opt/bracket.hpp"
#include "y/math/opt/minimize.hpp"

namespace upsylon {
    
    namespace math {
        
        namespace Adjust {

          


            //==================================================================
            //
            //
            //! utilities for LeastSquares
            //
            //
            //==================================================================
            template <typename T> struct Algo
            {
                typedef typename Type<T>::Matrix   Matrix; //!< alias

                //! 10^MinPower => 0
                static inline unit_t MinPower() throw()
                {
                    return -unit_t(numeric<T>::dig)-1;
                }

                //! 10^MaxPower+1 => failure
                static inline unit_t MaxPower() throw()
                {
                    return unit_t(numeric<T>::max_10_exp);
                }

                //! a starting value
                static inline unit_t Initial() throw()
                {
                    return -4;
                }
                
                //! correct curvature computation
                static inline
                bool ComputeCurvature(Matrix       &curv,
                                      const T       lambda,
                                      const Matrix &alpha) throw()
                {
                    assert(lambda>=0);
                    assert(alpha.is_square);
                    assert(curv.same_size_than(alpha));
                    
                    static const T one = 1;
                    const T        fac = one + lambda;
                    const size_t   n   = alpha.rows;
                    
                    for(size_t i=n;i>0;--i)
                    {
                        addressable<T>  &curv_i = curv[i];
                        size_t j=n;
                        for(;j>i;--j)
                        {
                            curv_i[j] = alpha[i][j];
                        }
                        assert(j==i);
                        curv_i[i] = fac * alpha[i][i];
                        for(--j;j>0;--j)
                        {
                            curv_i[j] = alpha[i][j];
                        }
                    }
                    return LU::build(curv);
                }

                //! compute full step from descent direction and curvature
                static inline
                void ComputeStep(addressable<T>       &step,
                                 const Matrix         &curv,
                                 const accessible<T>  &beta ) throw()
                {
                    atom::set(step,beta);
                    LU::solve(curv,step);
                }
                
                
            };

            //! conditional println
#define Y_LS_PRINTLN(OUTPUT) do{ if(verbose) { std::cerr << OUTPUT << std::endl; } } while(false)

            //==================================================================
            //
            //
            //! LeastSquares
            //
            //
            //==================================================================
            template <typename T>
            class LeastSquares : public Gradient<T>
            {
            public:
                typedef typename Type<T>::Matrix         Matrix;   //!< alias
                typedef typename Type<T>::Vector         Vector;   //!< alias
                typedef typename Type<T>::Function       Function; //!< alias
                typedef          Oxide::Field1D<T>       Field;    //!< alias

                typedef typename Context<T>::Modify      Modify;   //!< alias



                //! setup
                inline explicit LeastSquares(const bool verb=false) :
                verbose(verb),
                lambdas("lambda",Algo<T>::MinPower(),Algo<T>::MaxPower()),
                pmin(lambdas.lower),
                pmax(lambdas.upper),
                p(0),
                lambda(0),
                alpha(),
                beta(),
                curv(),
                step(),
                atry(),
                used(),
                nope( this, & LeastSquares<T>::doNothing )
                {
                    setup();
                }

                //! cleanup
                inline virtual ~LeastSquares() throw() {}
                
                bool         verbose; //!< activate verbosity
                const Field  lambdas; //!< precomputed lambdas
                const unit_t pmin;    //!< min power value
                const unit_t pmax;    //!< max power value
                

                //! fit wrapper
                inline bool fit(SampleType<T>            &sample,
                                Function                 &F,
                                addressable<T>           &aorg,
                                const accessible<bool>   &flags,
                                addressable<T>          &aerr,
                                Modify                   *modify = 0
                                )
                {
                    SequentialFunction<T> SF(F);
                    return fit(sample,SF,aorg,flags,aerr,modify);
                }


                //! full fit algorithm and error computation
                inline bool fit(SampleType<T>            &sample,
                                Sequential<T>            &F,
                                addressable<T>           &aorg,
                                const accessible<bool>   &flags,
                                addressable<T>           &aerr,
                                Modify                   *modify = 0 )
                {
                    static const T D_FTOL = numeric<T>::sqrt_ftol;
                    static const T A_FTOL = numeric<T>::ftol;

                    assert( flags.size() == aorg.size() );
                    assert( flags.size() == aerr.size() );

                    //__________________________________________________________
                    //
                    Y_LS_PRINTLN( "[LS] initializing" );
                    //__________________________________________________________
                    sample.ready();
                    atom::ld(aerr,-1);
                    const size_t n = aorg.size();
                    if(n<=0)
                    {
                        Y_LS_PRINTLN("[LS] <no parameters>");
                        return true;
                    }

                    //__________________________________________________________
                    //
                    Y_LS_PRINTLN( "[LS] starting with #parameters=" << n);
                    //__________________________________________________________
                    alpha.make(n,n);
                    beta.adjust(n,0);
                    curv.make(n,n);
                    step.adjust(n,0);
                    atry.adjust(n,0);
                    used.adjust(n,0);
                    atom::ld(used,false);
                    sample.activate(used,flags);
                    setLambda( Algo<T>::Initial() );

                    Y_LS_PRINTLN( "     flags  = " << flags );
                    Y_LS_PRINTLN( "     used   = " << used  );

                    //__________________________________________________________
                    //
                    // create the call function
                    //__________________________________________________________
                    ProbeD2 D2 = { &sample, &F, &atry, &aorg, &step};

                    //__________________________________________________________
                    //
                    // create the context
                    //__________________________________________________________
                    Context<T> context(sample,aorg,used,atry,step);
                    size_t     cycle = aliasing::_(context.cycle);
                    Modify    &check = (0!=modify) ? *modify : nope;
                    Y_LS_PRINTLN( "    #data   = " << context.size()  );

                    // starting point...
                    T      D2org = sample.computeD2(alpha, beta, F, aorg, used, *this);
                    size_t nbad  = 0;

                CYCLE:
                    ++cycle;
                    //__________________________________________________________
                    //
                    Y_LS_PRINTLN( "[LS] cycle  = " << cycle );

                    //__________________________________________________________



                    //__________________________________________________________
                    //
                    Y_LS_PRINTLN( "     used   = " << used  );
                    Y_LS_PRINTLN( "     aorg   = " << aorg );
                    Y_LS_PRINTLN( "     D2org  = " << D2org );
                    Y_LS_PRINTLN( "     alpha  = " << alpha );
                    Y_LS_PRINTLN( "     beta   = " << beta );
                    while( !Algo<T>::ComputeCurvature(curv,lambda,alpha) )
                    {
                        if(!increaseLambda())
                        {
                            Y_LS_PRINTLN( "[LS] <SINGULAR GRADIENT>" );
                            return false;
                        }
                    }
                    //__________________________________________________________

                    //__________________________________________________________
                    //
                    // compute step and trial position
                    //
                    Algo<T>::ComputeStep(step, curv, beta);
                    atom::add(atry,aorg,step);
                    Y_LS_PRINTLN( "     lambda = " << lambda );
                    Y_LS_PRINTLN( "     step0  = " << step   );
                    Y_LS_PRINTLN( "     atry0  = " << atry   );
                    //__________________________________________________________



                    //__________________________________________________________
                    //
                    // user control
                    switch( check(context) )
                    {
                        case LeftUntouched: //Y_LS_PRINTLN( "[LS] LeftUntouched" );
                            break;

                        case ModifiedState: Y_LS_PRINTLN( "[LS] ModifiedState" );
                            atom::sub(step,atry,aorg);
                            Y_LS_PRINTLN( "     atry   = " << atry   );
                            Y_LS_PRINTLN( "     step   = " << step   );
                            break;

                        case ModifiedShift: Y_LS_PRINTLN( "[LS] ModifiedShift" );
                            atom::add(atry,aorg,step);
                            Y_LS_PRINTLN( "     atry   = " << atry   );
                            Y_LS_PRINTLN( "     step   = " << step   );
                            break;

                        case EmergencyExit: Y_LS_PRINTLN( "[LS] EmergencyExit" );
                            return false;
                    }
                    //
                    //__________________________________________________________



                    //__________________________________________________________
                    //
                    //
                    // at this point, aorg, atry and step are computed
                    // now pinpoint a local minimum
                    //
                    //__________________________________________________________
                    triplet<T> u  = { 0,     numeric<T>::inv_gold, 1 };
                    triplet<T> f  = { D2org, D2(u.b),             -1 };
                    bool       ok = true;

                    if(f.b>f.a)
                    {
                        //------------------------------------------------------
                        //
                        // D2(u.b)>D2(u.a)
                        // first trial is invalid, won't go further !
                        //
                        //------------------------------------------------------
                        Y_LS_PRINTLN( "[LS] Backtrack Level-1" );
                        ok  = false;
                        f.c = f.b;
                        u.c = u.b;
                        bracket::inside(D2,u,f);
                        (void)minimize::run(D2,u,f);
                        if( f.b > f.a )
                        {
                            Y_LS_PRINTLN("[LS] <BAD FUNCTION>");
                            return false;
                        }
                        assert(f.b<=f.a);
                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        // D2(u.b) <= D2(u.a), take next step at u.c
                        //
                        //------------------------------------------------------
                        f.c = D2(u.c);
                        if(f.c>f.a)
                        {
                            //--------------------------------------------------
                            //
                            // D2(u.c=1)>D2(u.a=0) : invalid full step
                            // but minimum is already bracketed!
                            //
                            //--------------------------------------------------
                            ok = false;
                            Y_LS_PRINTLN( "[LS] Backtrack Level-2" );
                            (void)minimize::run(D2,u,f);
                            assert(f.b<=f.a);
                        }
                        else
                        {
                            //--------------------------------------------------
                            //
                            // D2(u.c=1)<=D2(u.a=0), accept full step
                            //
                            //--------------------------------------------------
                            if(f.b<=f.c)
                            {
                                // Damping! we met a lower point
                                (void)minimize::run(D2,u,f);
                                assert(f.b<=f.a);
                            }
                            else
                            {
                                // regular full step
                                u.b = u.c;
                                f.b = f.c;
                                assert(f.b<=f.a);
                            }
                        }
                    }

                    //__________________________________________________________
                    //
                    //
                    // u.b is the optimized step fraction
                    //
                    //__________________________________________________________

                    //----------------------------------------------------------
                    // compute the new position
                    //----------------------------------------------------------
                    atom::setprobe(atry,aorg,u.b,step);

                    //----------------------------------------------------------
                    // check variable convergence
                    //----------------------------------------------------------
                    bool converged_variables = true;
                    for(size_t i=n;i>0;--i)
                    {
                        if(!used[i]) continue;
                        const T a_old = aorg[i];
                        const T a_new = atry[i];
                        const T da    = fabs_of( a_new-a_old );
                        if( da > A_FTOL * max_of( fabs_of(a_new), fabs_of(a_old) ) )
                        {
                            converged_variables = false;
                            break;
                        }
                    }

                    //----------------------------------------------------------
                    // update aorg/D2org
                    //----------------------------------------------------------
                    atom::set(aorg,atry);
                    const T D2old = D2org;
                    D2org = sample.computeD2(alpha, beta, F, aorg, used, *this);

                    Y_LS_PRINTLN("[LS] D2: " << D2old << " -> " << D2org );
                    Y_LS_PRINTLN("[LS] converged variables = <" << converged_variables << ">");
                    if(ok)
                    {
                        //------------------------------------------------------
                        // check D2 convergence
                        //------------------------------------------------------
                        const bool converged_squares = (fabs_of(D2org-D2old) <= D_FTOL * D2old);
                        Y_LS_PRINTLN("[LS] converged squares   = <" << converged_squares   << ">");
                        if( converged_variables || converged_squares  )
                        {
                            goto CONVERGED;
                        }
                        decreaseLambda();
                        nbad = 0;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // check variable convergence
                        //------------------------------------------------------
                        ++nbad;
                        if( (nbad>0) && converged_variables )
                        {
                            Y_LS_PRINTLN("[LS] spurious convergence");
                            goto CONVERGED;
                        }

                        if( !increaseLambda() )
                        {
                            Y_LS_PRINTLN("[LS] <OVERFLOW>");
                            return false;
                        }
                    }
                    goto CYCLE;




                CONVERGED:
                    //__________________________________________________________
                    //
                    Y_LS_PRINTLN( "[LS] -- Converged --" );
                    Y_LS_PRINTLN( "     D2   = " << D2org );
                    Y_LS_PRINTLN( "     aorg = " << aorg  );
                    Y_LS_PRINTLN( "     used = " << used  );
                    //__________________________________________________________
                    if(!LU::build(alpha))
                    {
                        Y_LS_PRINTLN( "[LS] <SINGULAR Level-2>" );
                        return false;
                    }
                    LU::inverse(alpha,curv);

                    //__________________________________________________________
                    //
                    unit_t dof = sample.count();
                    Y_LS_PRINTLN( "    count = " << dof );
                    //__________________________________________________________
                    for(size_t i=n;i>0;--i)
                    {
                        if(used[i]) --dof;
                    }
                    Y_LS_PRINTLN( "      dof = " << dof );
                    if(dof<0)
                    {
                        Y_LS_PRINTLN( "[LS] <MEANINGLESS>" );
                        return false;
                    }
                    else if(0==dof)
                    {
                        Y_LS_PRINTLN( "[LS] <interpolation>" );
                        atom::ld(aerr,0);
                        return true;
                    }
                    else
                    {
                        assert(dof>0);
                        const T real_count = T(dof);
                        const T sig2err    = T(1)/sqrt_of(real_count);
                        for(size_t i=n;i>0;--i)
                        {
                            if(used[i])
                            {
                                const T sig = sqrt_of( (D2org * curv[i][i]) / dof );
                                aerr[i]     = sig*sig2err;
                            }
                            else
                            {
                                aerr[i] = 0;
                            }
                        }
                        Y_LS_PRINTLN( "     aerr = " << aerr );
                        return true;
                    }



                }
                
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(LeastSquares);
                unit_t   p;
                T        lambda;
                Matrix   alpha;
                Vector   beta;
                Matrix   curv;
                Vector   step;
                Vector   atry;
                bVector  used;
                Modify   nope;

                inline ModifyStatus doNothing( Context<T> & ) const
                {
                     return LeftUntouched;
                }
                
                inline void setup() throw()
                {
                    Field &lam = aliasing::_(lambdas);
                    lam[ pmin ] = 0;
                    {
                        static const T tenth(0.1);
                        lam[ -1 ] = tenth;
                        for(unit_t i=-2;i>pmin;--i) lam[i] = lam[i+1] * tenth;
                    }
                    lam[  0 ]            = 1;
                    {
                        static const T ten(10);
                        lam[  1 ]            = ten;
                        for(unit_t i=2;i<=pmax;++i) lam[i] = lam[i-1] * ten;
                    }
                    setLambda(0);
                }
                
                inline void setLambda( const unit_t p0 ) throw()
                {
                    lambda = lambdas[ (p=clamp(pmin,p0,pmax) ) ];
                }
                
                inline void decreaseLambda() throw()
                {
                    lambda = lambdas[ (p=max_of(p-1,pmin) ) ];
                    Y_LS_PRINTLN( "[LS] decreasing lambda" );
                }
                
                inline bool increaseLambda() throw()
                {
                    Y_LS_PRINTLN( "[LS] increasing lambda" );
                    if(p>=pmax)
                    {
                        Y_LS_PRINTLN( "   |_<OVERFLOW>" );
                        return false;
                    }
                    else
                    {
                        lambda = lambdas[ ++p ];
                        return true;
                    }
                }



                struct ProbeD2
                {
                    const SampleType<T> *sample;
                    Sequential<T>       *F_;
                    addressable<T>      *atry_;
                    const accessible<T> *aorg_;
                    const accessible<T> *step_;

                    inline T operator()(const T u)
                    {
                        assert(sample); assert(F_); assert(atry_); assert(aorg_); assert(step_);
                        atom::setprobe(*atry_, *aorg_, u, *step_);
                        return sample->computeD2(*F_, *atry_);
                    }

                };


            };
            
        }
    }
}


#endif

