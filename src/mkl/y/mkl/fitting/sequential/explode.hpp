

//! \file

#ifndef Y_MKL_FITTING_SEQUENTIAL_EXPLODE_INCLUDED
#define Y_MKL_FITTING_SEQUENTIAL_EXPLODE_INCLUDED 1

#include "y/mkl/fitting/sequential.hpp"
#include "y/mkl/ode/explicit/adjust.hpp"
#include "y/mkl/ode/explicit/driver-ck.hpp"

namespace upsylon {

    namespace mkl {

        namespace fitting {

            //! constructor for ExplODE
#define Y_EXPLODE_CTOR(SOLVER)         \
sequential_type(),                     \
solver(SOLVER),                        \
adjust(shared_adjust),                 \
diffEq( this, & explode<T>::compute ), \
fields( adjust->dimensions(), 0 ),     \
p_aorg(0),                             \
p_vars(0),                             \
__ctrl(0)

            //==================================================================
            //
            //
            //! Explicit ODE, sequential integration
            //
            //
            //==================================================================
            template <typename T>
            class explode : public sequential<T,T>
            {
            public:
                //==============================================================
                //
                // types and definitions
                //
                //==============================================================
                typedef sequential<T,T>               sequential_type; //!< alias
                typedef vector<T>                     ordinates;       //!< alias for internal data
                typedef ODE::ExplicitSolver<T>        solver_type;     //!< alias
                typedef arc_ptr<solver_type>          solver_ptr;      //!< alias
                typedef ODE::ExplicitAdjust<T>        adjust_type;     //!< alias
                typedef typename adjust_type::Pointer adjust_ptr;      //!< alias
                typedef ODE::DriverCK<T>              default_solver;  //!< default solver

                //==============================================================
                //
                //  members
                //
                //==================================================================

                //! cleanup
                inline virtual ~explode() throw()
                {

                }

                //! setup, preparing solver
                inline explicit explode(const solver_ptr &shared_solver,
                                        const adjust_ptr &shared_adjust) throw() :
                Y_EXPLODE_CTOR(shared_solver)
                {
                    setup();
                }


                //! setup with a private default solver
                inline explicit explode(const adjust_ptr &shared_adjust) throw() :
                Y_EXPLODE_CTOR( default_solver::New() )
                {
                    setup();
                }

                //! access current state
                const accessible<T> & operator*() const throw()
                {
                    return fields;
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(explode);
                solver_ptr                       solver;
                adjust_ptr                       adjust;
                typename ODE::Field<T>::Equation diffEq;
                ordinates                        fields;
                const accessible<T>             *p_aorg;
                const variables                 *p_vars;
                T                                __ctrl;

                //! finalize object
                void setup()
                {
                    solver->start( adjust->dimensions() );
                }


                //! ODE wrapper
                inline void compute( array<T> &dYdx, const T x, const array<T> &Y)
                {
                    assert(p_aorg);
                    assert(p_vars);
                    return adjust->rates(dYdx, x, Y, *p_aorg, *p_vars);
                }

                //! initialize protocol
                inline virtual T onStart(const T x1, const accessible<T> &aorg, const variables &vars)
                {
                    // link
                    p_aorg = &aorg;
                    p_vars = &vars;

                    // setup state and step control
                    adjust->setup(fields,aorg,vars);
                    __ctrl = adjust->delta();

                    // differential step: up to x1
                    (*solver)( diffEq, fields, adjust->start(), x1, __ctrl, adjust->callback() );

                    // done, return the value of interest
                    return adjust->query(x1,fields,aorg,vars);
                }

                //! update protocol
                inline virtual T onReach(const T x1, const accessible<T> &aorg, const variables &vars)
                {
                    // link
                    p_aorg = &aorg;
                    p_vars = &vars;


                    // differential step
                    (*solver)( diffEq, fields, this->current, x1, __ctrl,  adjust->callback());

                    // done, return the value of interest
                    return adjust->query(x1,fields,aorg,vars);
                }
            };

        }

    }

}

#endif

