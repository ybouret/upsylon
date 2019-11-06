
//! \file
#ifndef Y_MATH_FIT_LOCAL_VARIABLE_INCLUDED
#define Y_MATH_FIT_LOCAL_VARIABLE_INCLUDED 1

#include "y/math/fit/global-variable.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {

            ////////////////////////////////////////////////////////////////////
            //
            //! a local variable, linked to another variable
            //
            ////////////////////////////////////////////////////////////////////
            class LocalVariable : public Variable
            {
            public:
                explicit LocalVariable(const string &           __name,
                                       const Variable::Pointer &__link);  //!< constructor, attach to a link
                virtual ~LocalVariable() throw();                         //!< destructor
                virtual size_t index() const throw();                     //!< return link->index()
                const Variable::Pointer link;                             //!< the link

            private:
                Y_DISABLE_COPY_AND_ASSIGN(LocalVariable);
            };

        }
    }
}

#endif
