//! \file

#ifndef Y_MATH_ODE_PROBLEM_INCLUDED
#define Y_MATH_ODE_PROBLEM_INCLUDED 1

#include "y/math/ode/types.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            //! interface to solve a differential problem
            template <typename T>
            class Problem : public counted_object
            {
            public:
                inline virtual ~Problem() throw() {}

                virtual size_t dimension() const throw() = 0;                //!< problem dimension
                virtual void   setup( array<T> & ) const throw() = 0;        //!< initialize internal variables at initial coordinate
                virtual T      begin() const throw() = 0;                    //!< return linear=starting point/log=first log point
                virtual T      delta() const throw() = 0;                    //!< maximym coordinate step
                virtual T      query( const array<T> &, const T ) const = 0; //!< return final result at final coordinate

            protected:
                inline explicit Problem() throw() {}


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Problem);
            };
        }
    }
}

#endif

