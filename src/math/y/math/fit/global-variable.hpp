//! \file
#ifndef Y_MATH_FIT_GLOBAL_VARIABLE_INCLUDED
#define Y_MATH_FIT_GLOBAL_VARIABLE_INCLUDED 1

#include "y/math/fit/variable.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            ////////////////////////////////////////////////////////////////////
            //
            //! a global,top level variable
            //
            ////////////////////////////////////////////////////////////////////
            class GlobalVariable : public Variable
            {
            public:
                explicit GlobalVariable(const string &__name,
                                        const size_t  __indx); //! constructor, index must be > 0
                virtual ~GlobalVariable() throw();             //!< destructor
                virtual size_t index() const throw();          //!< return indx

                const size_t indx; //!< global index

            private:
                Y_DISABLE_COPY_AND_ASSIGN(GlobalVariable);
            };
            
        }
    }
}

#endif

