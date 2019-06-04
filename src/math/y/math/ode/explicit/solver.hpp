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
            //! explicit solver interface
            /**
             needs to be prepared before integration.
             */
            template <typename T>
            class ExplicitSolver : public counted_object, public SolverData<T>
            {
            public:
                typedef arc_ptr< ExplicitSolver<T> > Pointer;  //!< shared solver alias
                typedef typename Field<T>::Equation  Equation; //!< equation alias
                typedef typename Field<T>::Callback  Callback; //!< callback alias
                
                virtual ~ExplicitSolver() throw();  //!< cleanup

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
                explicit ExplicitSolver(); //!< setup

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
                void step(Equation              &drvs,
                          ExplicitControler<T>  &ctrl,
                          ExplicitStep<T>       &forward,
                          array<T>              &ystart,
                          const T                x1,
                          const T                x2,
                          T                     &h1,
                          Callback              *cb
                          );
            private:
                Y_DISABLE_COPY_AND_ASSIGN(ExplicitSolver);
            };


            //! parametrized driver that implements an explicit solver
            template <
            typename T,
            template <typename> class CTRL,
            template <typename> class STEP
            > class ExplicitDriver :
            public ExplicitSolver<T>
            {
            public:
                typedef typename Field<T>::Equation Equation; //!< equation alias
                typedef typename Field<T>::Callback Callback; //!< callback alias

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
                inline void operator()(Equation   &drvs,
                                       array<T>   &ystart,
                                       const T     x1,
                                       const T     x2,
                                       T          &h1,
                                       Callback   *cb
                                       )
                {
                    this->step( drvs, ctrl_, step_, ystart, x1, x2, h1,cb);
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
