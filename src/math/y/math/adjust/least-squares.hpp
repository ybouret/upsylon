//! \file

#ifndef Y_ADJUST_LEAST_SQUARES_INCLUDED
#define Y_ADJUST_LEAST_SQUARES_INCLUDED 1

#include "y/math/adjust/frame.hpp"
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
                typedef typename Context<T>::Control     Control;  //!< alias
                

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
                nope( this, & LeastSquares<T>::accept )
                {
                    initialize();
                }

                //! cleanup
                inline virtual ~LeastSquares() throw() {}
                
                bool         verbose; //!< activate verbosity
                const Field  lambdas; //!< precomputed lambdas
                const unit_t pmin;    //!< min power value
                const unit_t pmax;    //!< max power value
                
#include "least-squares-fit.hxx"

                 //! fit wrapper
                inline bool fit(SampleType<T>            &sample,
                                Function                 &F,
                                addressable<T>           &aorg,
                                const accessible<bool>   &flags,
                                addressable<T>          &aerr,
                                Control                  *modify = 0
                                )
                {
                    SequentialFunction<T> SF(F);
                    return fit(sample,SF,aorg,flags,aerr,modify);
                }

                //! return the current descent direction
                const accessible<T> & descent(SampleType<T>            &sample,
                                              Sequential<T>            &F,
                                              addressable<T>           &aorg,
                                              const accessible<bool>   &flags)
                {
                    allocateFor(sample,flags);
                    (void)sample.computeD2(alpha, beta, F, aorg, used, *this);
                    return beta;
                }

                //! return the current descent direction
                const accessible<T> & descent(SampleType<T>            &sample,
                                              Function                 &F,
                                              addressable<T>           &aorg,
                                              const accessible<bool>   &flags)
                {
                    SequentialFunction<T> SF(F);
                    return descent(sample,SF,aorg,flags);
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
                Control  nope;

                ContextStatus accept( Context<T> &, const size_t ) const throw()
                {
                    return ContextSuccess;
                }

                // prepare all internal memory
                inline void allocateFor(const SampleType<T>    &sample,
                                        const accessible<bool> &flags )
                {
                    const size_t n =  flags.size();
                    alpha.make(n,n);
                    beta.adjust(n,0);
                    curv.make(n,n);
                    step.adjust(n,0);
                    atry.adjust(n,0);
                    used.adjust(n,0);
                    atom::ld(used,false);
                    sample.activate(used,flags);
                }

                // initialize
                inline void initialize() throw()
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

