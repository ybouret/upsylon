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
            template <typename T>
            class ExplicitProblem : public Problem<T>
            {
            public:
                inline virtual ~ExplicitProblem() throw() {}

            protected:
                inline explicit ExplicitProblem() throw() : Problem<T>() {}
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(ExplicitProblem);
            };
        }

    }
}

#endif


