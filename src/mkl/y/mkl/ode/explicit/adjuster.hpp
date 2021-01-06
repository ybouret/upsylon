//! \file

#ifndef Y_MKL_ODE_EXPLICIT_ADJUSTER_INCLUDED
#define Y_MKL_ODE_EXPLICIT_ADJUSTER_INCLUDED 1

#include "y/mkl/ode/adjuster.hpp"

namespace upsylon {

    namespace mkl {

        namespace ODE {

            //__________________________________________________________________
            //
            //
            //! a problem to be solved with an explicit solver
            //
            //__________________________________________________________________
            template <typename T> class ExplicitAdjuster : public Adjuster<T>
            {
            public:
                typedef arc_ptr< ExplicitAdjuster<T> > Pointer;             //!< alias
                inline virtual ~ExplicitAdjuster() throw() {}               //!< destructor
            protected:
                inline explicit ExplicitAdjuster() throw() : Adjuster<T>() {} //!<constructor
            private:
                Y_DISABLE_COPY_AND_ASSIGN(ExplicitAdjuster);
            };
        }

    }
}

#endif

