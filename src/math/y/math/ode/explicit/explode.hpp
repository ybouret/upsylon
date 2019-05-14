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

            //__________________________________________________________________
            //
            //
            //! Explicit Ordinary Differential Equation Integrator
            //
            //__________________________________________________________________
            template <typename T>
            class ExplODE
            {
            public:
                //______________________________________________________________
                //
                // types definitions
                //______________________________________________________________
                Y_DECL_ARGS(T,type);                              //!< aliases
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

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //______________________________________________________________
                //
                //! cleanup
                //______________________________________________________________
                inline virtual ~ExplODE() throw() {}

                //______________________________________________________________
                //
                //! reset solver, return phase space
                //______________________________________________________________
                const array<type> & reset() { return initialize(); }
                
                //______________________________________________________________
                //
                //! perform integration from begin() to value, return phase space
                //______________________________________________________________
                const array<type> & at(const_type value)
                {
                    LSSI::LinearRun<type,SolverType>(*_solver,_diffeq,initialize(),_crunch->begin(),value,_crunch->delta(),_crunch->conform());
                    _current = value;
                    return _fields;
                }
                
                //______________________________________________________________
                //
                //! sequential call
                //______________________________________________________________
                const array<type> & update(const_type value)
                {
                    _solver->start(_fields.size());
                    LSSI::LinearRun<type,SolverType>(*_solver,_diffeq,_fields,_current,value,_crunch->delta(),_crunch->conform());
                    _current = value;
                    return _fields;
                }

                //______________________________________________________________
                //
                //! setup: acquire resources and set variables
                //______________________________________________________________
                inline explicit ExplODE(const Solver  &solver,
                                        const Problem &crunch) :
                _current(0),
                _solver(solver),
                _crunch(crunch),
                _diffeq( & *_crunch, & ProblemType::compute ),
                _fields(_crunch->dimension(),0)
                {
                    (void)reset();
                }

                //______________________________________________________________
                //
                // access last values
                //______________________________________________________________
                inline type                current() const throw() { return _current; } //!< current position
                inline const array<type> & fields() const throw()  { return _fields;  } //!< current phase space
                
            private:
                mutable_type   _current;
                Solver         _solver;
                Problem        _crunch;
                Equation       _diffeq;
                vector<T>      _fields;

                inline array<type> & initialize()
                {
                    _solver->start(_fields.size());
                    _crunch->setup(_fields);
                    _current = _crunch->begin();
                    return _fields;
                }

                Y_DISABLE_COPY_AND_ASSIGN(ExplODE);
            };

        }
    }
}

#endif

