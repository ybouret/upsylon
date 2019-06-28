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
                //
                // types definitions
                //
                //______________________________________________________________
                Y_DECL_ARGS(T,type);                              //!< aliases
                typedef ExplicitProblem<T>           ProblemType; //!< kind of problem
                typedef arc_ptr<ProblemType>         Problem;     //!< shared problem
                typedef typename Field<T>::Equation  Equation;    //!< alias for equation
                typedef typename Field<T>::Callback  Callback;    //!< alias for callback
                typedef typename Field<T>::Collect   Collect;     //!< alias for collect
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
                //
                // methods
                //
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
                //! perform integration from begin() to value
                /**
                 initialize at begin(), then solve up to value using internal
                 problem->delta() to control
                 */
                //______________________________________________________________
                const array<type> & at(const_type value, Collect *com=0)
                {
                    LSSI::LinearRun<type,SolverType>(*_solver,_diffeq,initialize(),_crunch->begin(),value,_crunch->delta(),_crunch->conform(),com);
                    return fields_at(value);
                }

                //______________________________________________________________
                //
                //! perform logarihtmic integration from begin() to value
                /**
                 intialize at begin(), then logarithminc run
                 using a first begin()->ln(vmin)
                 */
                //______________________________________________________________
                const array<type> & lnRun(const_type vmin, const_type vmax, Collect *com=0)
                {
                    assert(vmin>0);
                    assert(vmax>0);
                    const_type lnStep = _crunch->delta();
                    const type lnVmin = log_of(vmin);
                    const_type lnVmax = log_of(vmax);
                    LSSI::LogarithmicRun(*_solver,_diffeq,initialize(), lnVmin, lnVmax, lnStep, _crunch->conform(),com);
                    return fields_at(vmax);
                }

                //______________________________________________________________
                //
                //! sequential call
                //______________________________________________________________
                const array<type> & update(const_type value)
                {
                    assert( _fields.size() == _crunch->dimension() );
                    //_solver->start(_fields.size());
                    LSSI::LinearRun<type,SolverType>(*_solver,_diffeq,_fields,_current,value,_crunch->delta(),_crunch->conform());
                    return fields_at(value);
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

                inline const array<type> & fields_at( const_type value ) throw()
                {
                    _current = value;
                    return _fields;
                }

                Y_DISABLE_COPY_AND_ASSIGN(ExplODE);
            };

        }
    }
}

#endif

