
//! \file

#ifndef Y_MATH_ODE_EXPLICIT_PROBLEM_INCLUDED
#define Y_MATH_ODE_EXPLICIT_PROBLEM_INCLUDED 1

#include "y/math/ode/problem.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {

            //! interface to solve an explicit differential problem
            template <typename T>
            class ExplicitProblem : public Problem<T>
            {
            public:
                inline virtual ~ExplicitProblem() throw() {} //!< destructor

            protected:
                inline explicit ExplicitProblem() throw() : Problem<T>() {} //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(ExplicitProblem);
            };
           
        }
    }
}

#endif
