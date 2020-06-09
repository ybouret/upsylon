//! \file

#ifndef Y_MKL_FIT_SAMPLE_INFO_INCLUDED
#define Y_MKL_FIT_SAMPLE_INFO_INCLUDED 1

#include "y/mkl/fit/variables.hpp"

namespace upsylon  {

    namespace mkl {
        
        namespace Fit {

            //==================================================================
            //
            //
            //! interface for sample(s)
            //
            //
            //==================================================================
            class SampleInfo
            {
            public:
                //==============================================================
                //
                // members
                //
                //==============================================================
                Variables      variables;                 //!< variables for this sample

                //==============================================================
                //
                // virtual interface
                //
                //==============================================================
                virtual size_t count() const throw() = 0; //!< number of points
                virtual void   ready()               = 0; //!< make ready
                virtual       ~SampleInfo() throw();      //!< cleanup

            protected:
                explicit SampleInfo() throw(); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SampleInfo);
            };
        }
    }
}

#endif

