//! \file
#ifndef Y_MATH_ODE_PROBLEM_INCLUDED
#define Y_MATH_ODE_PROBLEM_INCLUDED 1

#include "y/math/ode/types.hpp"
#include "y/math/fit/variables.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            //! parametric equations
            template <typename T>
            class Problem : public counted_object
            {
            public:
                inline virtual ~Problem() throw() {}

                //! get the problem state size
                virtual size_t dimension() const throw() = 0;

                //! get the initial coordinate
                virtual T      start() const throw()     = 0;

                //! set the initial state ( at start() )
                virtual void   setup(array<T>             &Y,
                                     const array<T>       &aorg,
                                     const Fit::Variables &vars) const = 0;

                //! compute the rates
                virtual void   rates(array<T>             &dYdx,
                                     const T               x,
                                     const array<T>       &Y,
                                     const array<T>       &aorg,
                                     const Fit::Variables &vars) = 0;
                

                //! initial time step
                virtual T delta() const throw() = 0;

                //! query a scalar value from a state
                virtual T query(const T               x,
                                const array<T>       &Y,
                                const array<T>       &aorg,
                                const Fit::Variables &vars) const = 0;

                //! callback to control trial
                inline virtual typename Field<T>::Callback * callback() { return 0; }
                
            protected:
                inline explicit Problem() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Problem);
            };
        }
    }
}

#endif

