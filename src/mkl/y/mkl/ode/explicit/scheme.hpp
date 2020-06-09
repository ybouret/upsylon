
//! \file
#ifndef Y_MATH_ODE_EXPLICIT_SCHEME_INCLUDED
#define Y_MATH_ODE_EXPLICIT_SCHEME_INCLUDED 1

#include "y/math/ode/scheme.hpp"

namespace upsylon {

    namespace math {

        namespace ODE {

            //==================================================================
            //
            //
            //! a problem to be solved with an explicit solver
            //
            //
            //==================================================================
            template <typename T> class ExplicitScheme : public Scheme<T>
            {
            public:
                typedef arc_ptr< ExplicitScheme<T> > Pointer;             //!< alias
                inline virtual ~ExplicitScheme() throw() {}               //!< destructor
            protected:
                inline explicit ExplicitScheme() throw() : Scheme<T>() {} //!<constructor
            private:
                Y_DISABLE_COPY_AND_ASSIGN(ExplicitScheme);
            };
        }

    }
}

#endif


