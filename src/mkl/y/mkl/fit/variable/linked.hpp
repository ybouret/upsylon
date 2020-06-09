//! \file

#ifndef Y_MATH_ADJUST_VARIABLE_LINKED_INCLUDED
#define Y_MATH_ADJUST_VARIABLE_LINKED_INCLUDED 1

#include "y/math/fit/variable.hpp"

namespace upsylon {

    namespace math {

        namespace Fit {

            //! variable linked to another variable, for shared variables
            class LinkedVariable : public Variable
            {
            public:

                explicit LinkedVariable(const string &n, const Variable::Pointer &l); //!< setup
                explicit LinkedVariable(const char   *n, const Variable::Pointer &l); //!< setup
                LinkedVariable(const LinkedVariable &other); //!< copy

                virtual ~LinkedVariable() throw(); //!< cleanup
                virtual size_t index() const throw(); //!< link->index()


                const Variable::Pointer link; //!< shared link


            private:
                Y_DISABLE_ASSIGN(LinkedVariable);
            };

        }
    }

}

#endif
