//! \file
#ifndef Y_MATH_ODE_EXPLICIT_SOLVER_INCLUDED
#define Y_MATH_ODE_EXPLICIT_SOLVER_INCLUDED 1

#include "y/math/ode/explicit/controler.hpp"
#include "y/math/ode/solver-data.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            //! explicit solver
            /**
             needs to be prepared before integration.
             */
            template <typename T>
            class ExplicitSolver : public SolverData<T>
            {
            public:
                typedef typename Field<T>::Equation equation; //!< equation alias
                typedef typename Field<T>::Callback callback; //!< callback alias
                
                explicit ExplicitSolver();
                virtual ~ExplicitSolver() throw();

                
                //! ystart at x1 -> x2
                /**
                 \param drvs    differential equation
                 \param ctrl    an adaptive controler
                 \param forward an embedded step
                 \param ystart  initial values
                 \param x1      initial parameter
                 \param x2      final   parameter
                 \param h1      guess step, updated
                 \param cb      a callback to correct trials
                 */
                void operator()(equation              &drvs,
                                ExplicitControler<T>  &ctrl,
                                ExplicitStep<T>       &forward,
                                array<T>              &ystart,
                                const T                x1,
                                const T                x2,
                                T                     &h1,
                                callback              *cb
                                );



            private:
                Y_DISABLE_COPY_AND_ASSIGN(ExplicitSolver);
            };

            //! interface for explicit solver
            template <typename T>
            class ExplicitSolverInterface : public counted_object
            {
            public:
                typedef arc_ptr< ExplicitSolverInterface<T> > Pointer; //!< alias for shared solvers
                inline virtual ~ExplicitSolverInterface() throw() {}   //!< cleanup

                virtual void start(size_t nvar) = 0; //!< allocate internal data

                //! call operator
                /**
                 \param drvs   the differential equation
                 \param ystart in/out vector of variables
                 \param x1     initial coordinate
                 \param x2     final  coordinate
                 \param h1     in/out control step
                 \param cb     optional callback to control trial positions
                 */
                virtual  void operator()(typename Field<T>::Equation &drvs,
                                         array<T>                    &ystart,
                                         const T                      x1,
                                         const T                      x2,
                                         T                           &h1,
                                         typename Field<T>::Callback *cb) = 0;


            protected:
                inline explicit ExplicitSolverInterface() throw() {} //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(ExplicitSolverInterface);
            };

            
            //! parametrized driver
            template <
            typename T,
            template <typename> class CTRL,
            template <typename> class STEP
            > class ExplicitDriver :
            public ExplicitSolverInterface<T>,
            public ExplicitSolver<T>
            {
            public:
                typedef typename Field<T>::Equation equation; //!< equation alias
                typedef typename Field<T>::Callback callback; //!< callback alias

                explicit ExplicitDriver() : ExplicitSolver<T>(), ctrl_(), step_() {}
                virtual ~ExplicitDriver() throw() {}

                //! allocate all memory
                inline virtual void start( size_t nvar )
                {
                    this->acquire( nvar );
                    ctrl_.acquire( nvar );
                    step_.acquire( nvar );
                }

                //! make a step
                inline void operator()(equation   &drvs,
                                       array<T>   &ystart,
                                       const T     x1,
                                       const T     x2,
                                       T          &h1,
                                       callback   *cb
                                       )
                {
                    ExplicitSolver<T> &self = *this;
                    self( drvs, ctrl_, step_, ystart, x1, x2, h1,cb);
                }
                

            private:
                CTRL<T> ctrl_;
                STEP<T> step_;
                Y_DISABLE_COPY_AND_ASSIGN(ExplicitDriver);
            };
            
            
        }
    }
}


#endif
