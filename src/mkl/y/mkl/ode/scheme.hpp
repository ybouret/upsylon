//! \file

#ifndef Y_MKL_ODE_SCHEME_INCLUDED
#define Y_MKL_ODE_SCHEME_INCLUDED 1

#include "y/mkl/ode/types.hpp"
#include "y/mkl/fit/variables.hpp"

namespace upsylon {

    namespace mkl {

        namespace ODE {

            //==================================================================
            //
            //
            //! parametric equations
            //
            //
            //==================================================================
            template <typename T>
            class Scheme : public counted_object
            {
            public:
                typedef Fit::Variables Variables; //!< alias

                //==============================================================
                //
                // interface
                //
                //==============================================================

                //! cleaunp
                inline virtual ~Scheme() throw() {}

                //! get the problem state size
                virtual size_t dimensions() const throw() = 0;

                //! get the initial coordinate
                virtual T      start() const throw()     = 0;

                //! set the initial state ( at start() )
                virtual void   setup(addressable<T>       &Y,
                                     const accessible<T>  &aorg,
                                     const Variables      &vars) const = 0;

                //! compute the rates
                virtual void   rates(addressable<T>       &dYdx,
                                     const T               x,
                                     const accessible<T>  &Y,
                                     const accessible<T>  &aorg,
                                     const Variables      &vars) const = 0;


                //! initial time step
                virtual T delta() const throw() = 0;

                //! query a scalar value from a state
                virtual T query(const T              x,
                                const accessible<T> &Y,
                                const accessible<T> &aorg,
                                const Variables     &vars) const = 0;
                
                //! callback to control trial, default is none
                inline virtual typename Field<T>::Callback * callback() { return 0; }

            protected:
                //! setup
                inline explicit Scheme() throw() : counted_object() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Scheme);
            };
        }
    }
}

#endif

