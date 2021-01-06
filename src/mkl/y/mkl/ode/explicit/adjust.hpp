//! \file

#ifndef Y_MKL_ODE_EXPLICIT_ADJUSTER_INCLUDED
#define Y_MKL_ODE_EXPLICIT_ADJUSTER_INCLUDED 1

#include "y/mkl/ode/adjust.hpp"

namespace upsylon {

    namespace mkl {

        namespace ODE {

            //__________________________________________________________________
            //
            //
            //! a problem to be solved with an explicit solver
            //
            //__________________________________________________________________
            template <typename T> class ExplicitAdjust : public Adjust<T>
            {
            public:
                typedef arc_ptr< ExplicitAdjust<T> > Pointer;             //!< alias
                inline virtual ~ExplicitAdjust() throw() {}               //!< destructor
            protected:
                inline explicit ExplicitAdjust() throw() : Adjust<T>() {} //!<constructor
            private:
                Y_DISABLE_COPY_AND_ASSIGN(ExplicitAdjust);
            };
        }

    }
}

#endif

