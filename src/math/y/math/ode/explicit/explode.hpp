//! \file
#ifndef Y_MATH_ODE_EXPLODE_INCLUDED
#define Y_MATH_ODE_EXPLODE_INCLUDED 1

#include "y/math/ode/explicit/problem.hpp"
#include "y/math/ode/explicit/driver-ck.hpp"
#include "y/math/fit/types.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {

            //! macro to setup ExplODE
#define Y_MATH_EXPLODE_CTOR(ARG)       \
ExplicitSolver<T>::Pointer( ARG ),     \
crunch( host ),                        \
diffEq( this, & ExplODE<T>::compute ), \
fields( crunch.dimension(), 0 ),       \
p_aorg(0),                             \
p_vars(0),                             \
x_ctrl(0)

            //! parametric ODE sequential integrator, to use in LeastSquares
            template <typename T>
            class ExplODE :
            public ExplicitSolver<T>::Pointer,
            public Fit::Type<T>::Sequential
            {
            public:
                //! cleanup
                inline virtual ~ExplODE() throw()
                {
                    
                }

                //! setup with shared solver
                explicit ExplODE(ExplicitProblem<T>                        &host,
                                 const typename ExplicitSolver<T>::Pointer &esp ) throw() :
                Y_MATH_EXPLODE_CTOR(esp)
                {
                    finalize();
                }

                //! setup with new solver, or fallback
                explicit ExplODE(ExplicitProblem<T> &host,
                                 ExplicitSolver<T>  *esp = 0 ) throw() :
                Y_MATH_EXPLODE_CTOR( esp ? esp : DriverCK<T>::New() )
                {
                    finalize();
                }


            private:
                ExplicitProblem<T>           &crunch;
                typename Field<T>::Equation   diffEq;
                vector<T>                     fields;
                const array<T>               *p_aorg;
                const Fit::Variables         *p_vars;
                T                             x_ctrl;

                //! finalize construct
                inline void finalize() { (**this).start( crunch.dimension() ); }

                //! ODE wrapper
                inline void compute( array<T> &dYdx, const T x, const array<T> &Y)
                {
                    assert(p_aorg);
                    assert(p_vars);
                    return crunch.rates(dYdx, x, Y, *p_aorg, *p_vars);
                }

                //! initialize protocol
                inline virtual T on_initialize(const T x1, const array<T> &aorg, const Fit::Variables &vars)
                {
                    // link
                    p_aorg = &aorg;
                    p_vars = &vars;

                    // setup state and step
                    crunch.setup(fields,aorg,vars);
                    x_ctrl = crunch.delta();

                    // differential step up to x1
                    (**this)( diffEq, fields, crunch.start(), x1, x_ctrl, crunch.callback() );

                    // done
                    return crunch.query(x1,fields,aorg,vars);
                }

                //! update protocol
                inline virtual T on_compute_to(const T x1, const array<T> &aorg, const Fit::Variables &vars)
                {
                    // link
                    p_aorg = &aorg;
                    p_vars = &vars;


                    // differential step
                    (**this)( diffEq, fields, this->current, x1, x_ctrl,  crunch.callback());

                    // done
                    return crunch.query(x1,fields,aorg,vars);
                }
                Y_DISABLE_COPY_AND_ASSIGN(ExplODE);
            };

        }
    }

}

#endif

