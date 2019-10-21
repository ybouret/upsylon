//! \file

#ifndef Y_MATH_SAMPLE_INFO_INCLUDED
#define Y_MATH_SMAPLE_INFO_INCLUDED 1

#include "y/math/adjust/types.hpp"

namespace upsylon  {

    namespace math {
        
        namespace Adjust {

            class SampleInfo
            {
            public:
                
                virtual size_t count() const throw() = 0; //!< number of points
                virtual void   ready()               = 0; //!< make ready

                virtual ~SampleInfo() throw();

            protected:
                explicit SampleInfo() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SampleInfo);
            };
        }
    }
}

#endif

