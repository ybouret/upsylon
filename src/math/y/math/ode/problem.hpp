//! \file

#ifndef Y_MATH_ODE_PROBLEM_INCLUDED
#define Y_MATH_ODE_PROBLEM_INCLUDED 1

#include "y/math/ode/types.hpp"
#include "y/ptr/arc.hpp"
#include "y/ptr/embedded.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            //! interface to solve a differential problem
            /**
             Using this approach, one can transform a differential equation
             into an integrated function for further use.
             */
            template <typename T>
            class Problem : public counted_object
            {
            public:
                typedef typename Field<T>::Callback Callback; //!< alias

                //______________________________________________________________
                //
                // types
                //______________________________________________________________
                Y_DECL_ARGS(T,type);                 //!< aliases for typename

                //______________________________________________________________
                //
                // (virtual) interface
                //______________________________________________________________
                inline virtual   ~Problem() throw() {}                           //!< cleanup
                virtual size_t    dimension() const throw()                 = 0; //!< problem dimension
                virtual void      setup( array<T> & ) const throw()         = 0; //!< initialize internal variables at initial coordinate
                virtual T         begin() const throw()                     = 0; //!< return linear=starting point/log=first log point
                virtual T         delta() const throw()                     = 0; //!< maximum coordinate step
                virtual void      compute( array<T> &, T, const array<T> &) = 0; //!< differential equation API
                virtual Callback *conform() throw() { return NULL; }             //!< default callback


            protected:
                inline explicit Problem() throw() {} //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Problem);
            };

            
        }
    }
}

#endif

