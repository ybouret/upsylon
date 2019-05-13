//! \file
#ifndef Y_MATH_ODE_ExODE_INCLUDED
#define Y_MATH_ODE_ExODE_INCLUDED 1

#include "y/math/ode/explicit/lssi.hpp"
#include "y/math/ode/explicit/problem.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {

            //! Integrated Ordinary Differential Equation
            template <typename T>
            class ExODE
            {
            public:
                typedef ExplicitProblem<T>           ProblemType;
                typedef arc_ptr<ProblemType>         Problem;
                typedef typename Field<T>::Equation  Equation;
                typedef typename Field<T>::Callback  Callback;

                typedef ExplicitSolverInterface<T>   ESI;
                typedef typename ESI::Pointer        Solver;


                virtual ~ExODE() throw() {}


                //! perform integration from begin() to value
                inline T at(const T value)
                {
                    S->start( Y.size() );
                    P->setup(Y);
                    LSSI::LinearRun(*S,E,Y,P->begin(),value,P->delta(),NULL);
                    return P->query(Y,value);
                }

                //! perform integration from 0 to exp(lnValue), first step is [0,begin()]
                inline T at_log(const T lnValue)
                {
                    S->start( Y.size() );
                    P->setup(Y);
                    LSSI::LogarithmicRun(*S,E,Y,P->begin(),lnValue,P->delta(),NULL);
                    return P->query(Y,exp(lnValue));
                }


                //! setup
                inline explicit ExODE(const Solver  &solver,
                                      const Problem &problem) :
                S(solver),
                P(problem),
                E( & *P, & ProblemType::compute ),
                Y( P->dimension(),0)
                {
                    S->start( Y.size() );
                }


            private:
                Solver         S;
                Problem        P;
                Equation       E;
                vector<T>      Y;


                Y_DISABLE_COPY_AND_ASSIGN(ExODE);
            };

        }
    }
}

#endif

