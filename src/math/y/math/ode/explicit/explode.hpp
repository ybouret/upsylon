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
                typedef ExplicitSolver<T>            SolverType;  //!< alias
                typedef typename SolverType::Pointer Solver;      //!< alias for shared solver

                //! embedding derived problem
                /**
                 the PROBLEM must derive from a ProblemType and implement
                 the (virtual) interface
                 */
                template <typename PROBLEM>
                struct Embedded
                {
                    typedef embedded<PROBLEM,ProblemType,arc_ptr> Type; //!< ensure conversion
                };


                inline virtual ~ExplODE() throw() {} //!< cleanup

                //! perform integration from begin() to value, return phase space
                const array<T> & at(const T value)
                {
                    S->start( Y.size() );
                    P->setup(Y);
                    LSSI::LinearRun(*S,E,Y,P->begin(),value,P->delta(),NULL);
                    last_ = value;
                    return Y;
                }
                
                //! reset solver
                const array<T> & reset()
                {
                    S->start( Y.size() );
                    P->setup(Y);
                    last_ = P->begin();
                    return Y;
                }
                
                //! sequential call
                const array<T> & update(const T value)
                {
                    S->start( Y.size() );
                    LSSI::LinearRun(*S,E,Y,last_,value,P->delta(),NULL);
                    last_ = value;
                    return Y;
                }

                //! setup
                inline explicit ExplODE(const Solver  &solver,
                                        const Problem &problem) :
                last_(0),
                S(solver),
                P(problem),
                E( & *P, & ProblemType::compute ),
                Y( P->dimension(),0)
                {
                    reset();
                }

                inline T last() const throw() { return last_; }
                

            private:
                T              last_;
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

