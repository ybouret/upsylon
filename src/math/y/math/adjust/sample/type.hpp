//! \file

#ifndef Y_MATH_ADJUST_SAMPLE_TYPE_INCLUDED
#define Y_MATH_ADJUST_SAMPLE_TYPE_INCLUDED 1

#include "y/math/adjust/sample/info.hpp"
#include "y/math/adjust/sequential/function.hpp"
#include "y/math/adjust/sequential/gradient.hpp"
#include "y/math/kernel/atom.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            
            //==================================================================
            //
            //
            //! interface for adjustable sample
            //
            //
            //==================================================================
            template <typename T>
            class SampleType : public SampleInfo
            {
            public:
                //==============================================================
                //
                // types and definitions
                //
                //==============================================================
                typedef typename Type<T>::Matrix     Matrix;       //!< alias
                typedef typename Type<T>::Function   Function;     //!< alias

                //==============================================================
                //
                // virtual interface
                //
                //==============================================================

                //! cleanup
                inline virtual ~SampleType() throw() {}

                //! compute least squares with a sequential function
                virtual T compute(Sequential<T> &F, const accessible<T> &aorg) const = 0;

                //! full computation with a sequential function
                virtual T  computeAndUpdate(Matrix                 &alpha,
                                            addressable<T>         &beta,
                                            Sequential<T>          &F,
                                            const accessible<T>    &aorg,
                                            const accessible<bool> &used,
                                            Gradient<T>            &grad) const = 0;

                //! flags activation
                virtual void activate(addressable<bool>      &target,
                                      const accessible<bool> &source) const = 0;

                //! computing quantities for goodness of fit
                virtual void addToSumOfSquares(T &total,
                                               T &residual) const throw() = 0;
                
                //! upper abscissa, after a ready()!
                virtual T upper() const throw() = 0;
                
                
                //! upper abscissa, after a ready()!
                virtual T lower() const throw() = 0;

                //==============================================================
                //
                // non virtual interface
                //
                //==============================================================

                //! initialize matrix/vector from used statius
                inline void initialize(Matrix                 &alpha,
                                       addressable<T>         &beta,
                                       const accessible<bool> &used ) const throw()
                {
                    assert( alpha.rows  == alpha.cols  );
                    assert( beta.size() == alpha.rows );
                    assert( used.size() == beta.size() );
                    assert( variables.sweep() <= used.size() );

                    // assuming no one is used
                    atom::ld(beta,0);
                    alpha.Id();

                    // cleanup used variables
                    size_t                    nv = variables.size();
                    Variables::const_iterator it = variables.begin();
                    while(nv>0)
                    {
                        const size_t i = (**it).index();
                        if( used[i] )
                        {
                            alpha[i][i] = 0;
                        }
                        --nv;
                        ++it;
                    }
                }

                //! wrapper to use a C++ function
                inline T computeD2( Function &F, const accessible<T>  &aorg ) const
                {
                    SequentialFunction<T> call(F);
                    return compute(call,aorg);
                }

                //! compute D2 overloaded
                inline T computeD2(Sequential<T>       &F,
                                   const accessible<T> &aorg) const
                {
                    return compute(F,aorg);
                }



                //! compute D2 overloaded
                inline T computeD2(Matrix                  &alpha,
                                   addressable<T>          &beta,
                                   Sequential<T>           &F,
                                   const accessible<T>     &aorg,
                                   const accessible<bool>  &used,
                                   Gradient<T>             &grad) const
                {
                    initialize(alpha,beta,used);
                    const T D2 = computeAndUpdate(alpha,beta,F,aorg,used,grad);
                    Type<T>::Regularize(alpha);
                    return D2;
                }

                //! compute coefficient of determination R2, after a computeD2
                inline T computeR2() const throw()
                {
                    T SStot = 0, SSres = 0;
                    addToSumOfSquares(SStot, SSres);
                    return T(1) - SSres/SStot;
                }


            protected:
                //! setup
                inline explicit SampleType() throw() : SampleInfo() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SampleType);

            };

        }

    }
}


#endif

