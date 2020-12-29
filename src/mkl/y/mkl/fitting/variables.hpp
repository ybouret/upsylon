
//! \file
#ifndef Y_FITTING_VARIABLES_INCLUDED
#define Y_FITTING_VARIABLES_INCLUDED 1

#include "y/mkl/fitting/variable/primary.hpp"
#include "y/mkl/fitting/variable/replica.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            class variables : public collection
            {
            public:
                virtual ~variables() throw();
                virtual size_t size() const throw();

            private:
            };

        }
    }
}

#endif


