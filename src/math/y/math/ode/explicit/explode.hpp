//! \file
#ifndef Y_MATH_ODE_ExODE_INCLUDED
#define Y_MATH_ODE_ExODE_INCLUDED 1

#include "y/math/ode/explicit/problem.hpp"
#include "y/math/ode/explicit/lssi.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {

            //! Integrated Ordinary Differential Equation
            template <typename T>
            class ExplODE
            {
            public:
                typedef ExplicitProblem<T>           ProblemType; //!< kind of problem
                typedef arc_ptr<ProblemType>         Problem;     //!< shared problem
                typedef typename Field<T>::Equation  Equation;    //!< alias for equation
                typedef typename Field<T>::Callback  Callback;    //!< alias for callback
                typedef ExplicitSolverInterface<T>   ESI;         //!< alias
                typedef typename ESI::Pointer        Solver;      //!< alias for shared sovler

                //! embedding derived problem
                template <typename PROBLEM>
                struct Embedded
                {
                    typedef embedded<PROBLEM,ProblemType,arc_ptr> Type; //!< ensure conversion
                };


                inline virtual ~ExplODE() throw() {} //!< cleanup

                //! perform integration from begin() to value, return phase space
                const array<T> & state_at(const T value )
                {
                    S->start( Y.size() );
                    P->setup(Y);
                    LSSI::LinearRun(*S,E,Y,P->begin(),value,P->delta(),NULL);
                    return Y;
                }

                //! perform integration from begin() to value
                inline T at(const T value)
                {
                    return P->query(state_at(value),value);
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
                inline explicit ExplODE(const Solver  &solver,
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


                Y_DISABLE_COPY_AND_ASSIGN(ExplODE);
            };

        }
    }
}

#endif

