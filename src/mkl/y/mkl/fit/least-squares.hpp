//! \file

#ifndef Y_ADJUST_LEAST_SQUARES_INCLUDED
#define Y_ADJUST_LEAST_SQUARES_INCLUDED 1

#include "y/math/fit/frame.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/math/opt/bracket.hpp"
#include "y/math/opt/minimize.hpp"

namespace upsylon {
    
    namespace math {
        
        namespace Fit {

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


                //! corrected curvature computation
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
                    quark::set(step,beta);
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
                //--------------------------------------------------------------
                //
                // types and definitions
                //
                //--------------------------------------------------------------
                typedef typename Type<T>::Matrix         Matrix;   //!< alias
                typedef typename Type<T>::Vector         Vector;   //!< alias
                typedef typename Type<T>::Function       Function; //!< alias
                typedef typename Frame<T>::Control       Control;  //!< alias
                typedef typename Frame<T>::Controls      Controls; //!< alias
                typedef vector<const T>                  SmallVec; //!< alias
                
                //--------------------------------------------------------------
                //
                // C++ ctor/dtor
                //
                //--------------------------------------------------------------

                //! setup
                inline explicit LeastSquares(const bool verb=false) :
                verbose(verb),
                pmin(Algo<T>::MinPower()),
                pmax(Algo<T>::MaxPower()),
                vlam(1+pmax-pmin,0),
                damp(1),
                lambdas( &vlam.front() - pmin ),
                p(0),
                lambda(0),
                alpha(),
                beta(),
                curv(),
                step(),
                atry(),
                used(),
                good(),
                nope( this, & LeastSquares<T>::accept )
                {
                    initialize();
                }

                //! cleanup
                inline virtual ~LeastSquares() throw() {}


                //--------------------------------------------------------------
                //
                // fit functions
                //
                //--------------------------------------------------------------
                
#include "least-squares-fit.hxx"

                //! fit wrapper
                inline bool fit(SampleType<T>            &sample,
                                Function                 &F,
                                addressable<T>           &aorg,
                                const accessible<bool>   &flags,
                                addressable<T>           &aerr,
                                Controls                 *ctrl = 0,
                                const size_t              nmax = 0
                                )
                {
                    SequentialFunction<T> SF(F);
                    return fit(sample,SF,aorg,flags,aerr,ctrl,nmax);
                }

                //! return the current descent direction
                const accessible<T> & descent(SampleType<T>            &sample,
                                              Sequential<T>            &F,
                                              const accessible<T>      &aorg,
                                              const accessible<bool>   &flags)
                {
                    allocateFor(sample,flags);
                    (void)sample.computeD2(alpha, beta, F, aorg, used, *this);
                    return beta;
                }



                //! return the current descent direction
                const accessible<T> & descent(SampleType<T>            &sample,
                                              Function                 &F,
                                              const accessible<T>      &aorg,
                                              const accessible<bool>   &flags)
                {
                    SequentialFunction<T> SF(F);
                    return descent(sample,SF,aorg,flags);
                }

                //! return the current step
                const accessible<T> & stride(SampleType<T>            &sample,
                                             Sequential<T>            &F,
                                             const accessible<T>      &aorg,
                                             const accessible<bool>   &flags)
                {
                    allocateFor(sample,flags);
                    (void)sample.computeD2(alpha, beta, F, aorg, used, *this);
                    setLambda(pmin);
                    if(!buildStep()) throw exception("[LS] singular stride");
                    return step;
                }

                //! return the current step
                const accessible<T> & stride(SampleType<T>            &sample,
                                             Function                 &F,
                                             const accessible<T>      &aorg,
                                             const accessible<bool>   &flags)
                {
                    SequentialFunction<T> SF(F);
                    return stride(sample,SF,aorg,flags);
                }


                bool           verbose;   //!< activate verbosity
                const unit_t   pmin;     //!< min power value
                const unit_t   pmax;     //!< max power value
                const SmallVec vlam;     //!< precomputed lambdas
                const T        damp;     //!< damp factor to slow done search, default=1

                //! set damping factor into [0:1]
                inline void setDamp( const T dampValue ) throw()
                {
                    aliasing::_(damp) = clamp<T>(0,dampValue,1);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(LeastSquares);
                const T *lambdas;
                unit_t   p;
                T        lambda;
                Matrix   alpha;
                Vector   beta;
                Matrix   curv;
                Vector   step;
                Vector   atry;
                bVector  used;
                bVector  good;
                Control  nope;

                ControlResult accept( Frame<T> &, const size_t ) const throw()
                {
                    return LeftUntouched;
                }


                //______________________________________________________________
                //
                // prepare all internal memory
                //______________________________________________________________
                inline void allocateFor(const SampleType<T>    &sample,
                                        const accessible<bool> &flags )
                {
                    const size_t n =  flags.size();
                    alpha.make(n,n);
                    beta.adjust(n,0);
                    curv.make(n,n);
                    step.adjust(n,0);
                    atry.adjust(n,0);
                    used.adjust(n,false);
                    good.adjust(n,false);
                    quark::ld(used,false);
                    sample.activate(used,flags);
                    quark::set(good,used);
                }

                //______________________________________________________________
                //
                // initialize lambdas
                //______________________________________________________________
                inline void initialize() throw()
                {
                    //Field &lam = aliasing::_(lambdas);
                    T *lam = (T*)lambdas;
                    lam[ pmin ] = 0;
                    {
                        static const T tenth = T(0.1);
                        lam[ -1 ] = tenth;
                        for(unit_t i=-2;i>pmin;--i) lam[i] = lam[i+1] * tenth;
                    }
                    lam[  0 ]            = 1;
                    {
                        static const T ten = T(10);
                        lam[  1 ]            = ten;
                        for(unit_t i=2;i<=pmax;++i) lam[i] = lam[i-1] * ten;
                    }
                    setLambda( Algo<T>::Initial() );
                }

                //______________________________________________________________
                //
                // auto set lambda
                //______________________________________________________________
                inline void setLambda( const unit_t p0 ) throw()
                {
                    lambda = lambdas[ (p=clamp(pmin,p0,pmax) ) ];
                }

                //______________________________________________________________
                //
                // safely decrease lambda
                //______________________________________________________________
                inline void decreaseLambda() throw()
                {
                    lambda = lambdas[ (p=max_of(p-1,pmin) ) ];
                    Y_LS_PRINTLN( "[LS] decreasing lambda" );
                }

                //______________________________________________________________
                //
                // safely increase lambda
                //______________________________________________________________
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

                //! detect a nullspace coordinate
                inline bool isNullSpace( const size_t i ) const throw()
                {
                    return (fabs_of( beta[i] ) <= 0) && ( quark::mmod2<T>::both_of(alpha,i,i) <=0 );
                }
                
                //______________________________________________________________
                //
                // check if some variables are in a nullspace
                // after a full step computation: alpha/beta
                // the matric alpha is regularized and good is set
                //______________________________________________________________
                inline void checkNullSpace(const Variables &vars)
                {
                    for(size_t i=used.size();i>0;--i)
                    {
                        bool &status = good[i];
                        if( !used[i] )
                        {
                            status = false;
                        }
                        else
                        {
                            status = true;
                            if( isNullSpace(i) )
                            {
                                const Variable *pV = vars.searchIndex(i);
                                if(!pV)
                                {
                                    throw exception("[LS]: internal error: undeclared used variable!");
                                }
                                Y_LS_PRINTLN( "[LS] <NULL SPACE for [" << pV->name << "] >" );
                                status = false;
                                alpha[i][i] = 1;
                            }
                        }
                    }
                }

                //______________________________________________________________
                //
                // build step from current alpha, lambda, beta
                //______________________________________________________________
                inline bool buildStep(const Variables &vars)
                {
                    //__________________________________________________________
                    //
                    // check if a variable is in the nullspace
                    //__________________________________________________________
                    checkNullSpace(vars);
                    
                    //__________________________________________________________
                    //
                    // find a regular matrix
                    //__________________________________________________________
                    while( !Algo<T>::ComputeCurvature(curv,lambda,alpha) )
                    {
                        if(!increaseLambda())
                        {
                            Y_LS_PRINTLN( "[LS] <SINGULAR CURVATURE>" );
                            return false;
                        }
                    }

                    //__________________________________________________________
                    //
                    // and compute step
                    //__________________________________________________________
                    Algo<T>::ComputeStep(step, curv, beta);

                    //__________________________________________________________
                    //
                    // and take caution
                    //__________________________________________________________
                    quark::rescale(step,damp);
                    return true;
                }

                struct ProbeD2
                {
                    const SampleType<T> *sample;
                    Sequential<T>       *F;
                    addressable<T>      *atry;
                    const accessible<T> *aorg;
                    const accessible<T> *step;

                    
                    inline T operator()(const T u)
                    {
                        assert(sample);
                        assert(F);
                        assert(atry);
                        assert(aorg);
                        assert(step);

                        quark::muladd(*atry, *aorg, u, *step);
                        return sample->computeD2(*F, *atry);
                    }

                };


            };
            
        }
    }
}


#endif

