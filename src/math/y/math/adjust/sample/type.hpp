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

            //! interface for adjustable sample
            template <typename T>
            class SampleType : public SampleInfo
            {
            public:
                typedef typename Type<T>::Array    Array;
                typedef typename Type<T>::Matrix   Matrix;
                typedef typename Type<T>::Function Function;
                
                inline virtual ~SampleType() throw() {}


                //! compute with a sequential function
                virtual T compute(Sequential<T> &F, const Array &aorg) const = 0;
                
                virtual T  computeAndUpdate(Matrix          &alpha,
                                            Array           &beta,
                                            Sequential<T>   &F,
                                            const Array     &aorg,
                                            const Flags     &used,
                                            Gradient<T>     &grad) const = 0;
                
                inline T compute_( Function &F, const Array &aorg ) const
                {
                    SequentialFunction<T> call(F);
                    return compute(call,aorg);
                }
                
                inline void initialize(Matrix      &alpha,
                                       Array       &beta,
                                       const Flags &used ) const throw()
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
                
                // non virtual interface
                
                inline T computeD2( Sequential<T>   &F,
                                   const Array     &aorg) const
                {
                    return compute(F,aorg);
                }
                
                inline T computeD2(Matrix          &alpha,
                                   Array           &beta,
                                   Sequential<T>   &F,
                                   const Array     &aorg,
                                   const Flags     &used,
                                   Gradient<T>     &grad) const
                {
                    initialize(alpha,beta,used);
                    const T D2 = computeAndUpdate(alpha,beta,F,aorg,used,grad);
                    Type<T>::Regularize(alpha);
                    return D2;
                }
                
              


            protected:
                inline explicit SampleType() throw() : SampleInfo() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SampleType);
            };

        }

    }
}


#endif

