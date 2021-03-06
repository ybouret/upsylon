
//! \file
#ifndef Y_MKL_ODE_EXPLICIT_SCHEME_INCLUDED
#define Y_MKL_ODE_EXPLICIT_SCHEME_INCLUDED 1

#include "y/mkl/ode/scheme.hpp"

namespace upsylon {

    namespace mkl {

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


