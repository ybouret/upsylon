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
            //! a problem to be solved with an explicit solver
            template <typename T>
            class ExplicitProblem : public Problem<T>
            {
            public:
                //! destructor
                inline virtual ~ExplicitProblem() throw() {}

            protected:
                //! constructor
                inline explicit ExplicitProblem() throw() : Problem<T>() {}
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(ExplicitProblem);
            };
        }

    }
}

#endif


