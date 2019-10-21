//! \file

#ifndef Y_MATH_ADJUST_SAMPLE_TYPE_INCLUDED
#define Y_MATH_ADJUST_SAMPLE_TYPE_INCLUDED 1

#include "y/math/adjust/sample/info.hpp"
#include "y/math/adjust/sequential/function.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            //! interface for adjustable sample
            template <typename T>
            class SampleType : public SampleInfo
            {
            public:
                typedef typename Type<T>::Array    Array;
                typedef typename Type<T>::Function Function;

                inline virtual ~SampleType() throw() {}


                //! compute with a sequential function
                virtual T compute(Sequential<T> &F, const Array &aorg) const = 0;
                
                inline T compute_( Function &F, const Array &aorg ) const
                {
                    SequentialFunction<T> call(F);
                    return compute(call,aorg);
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

