//! \file

#ifndef Y_MATH_ADJUST_SAMPLE_TYPE_INCLUDED
#define Y_MATH_ADJUST_SAMPLE_TYPE_INCLUDED 1

#include "y/math/adjust/sample/info.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            //! interface for adjustable sample
            template <typename T>
            class SampleType : public SampleInfo
            {
            public:
                inline virtual ~SampleType() throw() {}



            protected:
                inline explicit SampleType() throw() : SampleInfo() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SampleType);
            };

        }

    }
}


#endif

