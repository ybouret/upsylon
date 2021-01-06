//! \file

#ifndef Y_MKL_ODE_ADJUSTER_INCLUDED
#define Y_MKL_ODE_ADJUSTER_INCLUDED 1

#include "y/mkl/ode/types.hpp"
#include "y/mkl/fitting/variables.hpp"

namespace upsylon {

    namespace mkl {

        namespace ODE {

            //__________________________________________________________________
            //
            //
            //! parametric equations
            //
            //__________________________________________________________________
            template <typename T>
            class Adjuster : public counted_object
            {
            public:
                typedef fitting::variables variables; //!< alias

                //______________________________________________________________
                //
                // interface
                //
                //______________________________________________________________

                //! cleanup
                inline virtual ~Adjuster() throw() {}

                //! get the problem state size
                virtual size_t dimensions() const throw() = 0;

                //! get the initial coordinate
                virtual T      start() const throw()     = 0;

                //! set the initial state ( at start() )
                virtual void   setup(addressable<T>       &Y,
                                     const accessible<T>  &aorg,
                                     const variables      &vars) const = 0;

                //! compute the rates
                virtual void   rates(addressable<T>       &dYdx,
                                     const T               x,
                                     const accessible<T>  &Y,
                                     const accessible<T>  &aorg,
                                     const variables      &vars) const = 0;


                //! initial time step
                virtual T delta() const throw() = 0;

                //! query a scalar value from a state
                virtual T query(const T              x,
                                const accessible<T> &Y,
                                const accessible<T> &aorg,
                                const variables     &vars) const = 0;

                //! callback to control trial, default is none
                inline virtual typename Field<T>::Callback * callback() { return 0; }

            protected:
                //! setup
                inline explicit Adjuster() throw() : counted_object() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Adjuster);
            };
        }
    }
}

#endif

