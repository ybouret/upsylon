//! \file
#ifndef Y_MATH_FIT_SAMPLE_INFO_INCLUDED
#define Y_MATH_FIT_SAMPLE_INFO_INCLUDED 1

#include "y/math/fit/variables.hpp"


namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            ////////////////////////////////////////////////////////////////////
            //
            //! Common Sample information
            //
            ////////////////////////////////////////////////////////////////////
            class SampleInfo : public counted_object
            {
            public:
                //______________________________________________________________
                //
                // types
                //______________________________________________________________
                typedef   vector<size_t> Indices; //!< indices for ordered evaluations

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual size_t count() const throw() = 0; //!< number of points
                virtual void   prepare()             = 0; //!< prepare resources

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                Variables variables; //!< local or global variables

            protected: explicit SampleInfo(const size_t nvar_max);  //!< initialize
            public:    virtual ~SampleInfo() throw();               //!< destructor

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SampleInfo);
            };

        }
    }
}

#endif

