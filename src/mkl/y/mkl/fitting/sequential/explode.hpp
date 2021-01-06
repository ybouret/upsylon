

//! \file

#ifndef Y_MKL_FITTING_SEQUENTIAL_EXPLODE_INCLUDED
#define Y_MKL_FITTING_SEQUENTIAL_EXPLODE_INCLUDED 1

#include "y/mkl/fit/sequential.hpp"
#include "y/mkl/ode/explicit/adjuster.hpp"
#include "y/mkl/ode/explicit/driver-ck.hpp"

namespace upsylon {

    namespace mkl {

        namespace fitting {

            //! constructor for ExplODE
#define Y_EXPLODE_CTOR(SOLVER)         \
Sequential<T>(),                       \
solver(SOLVER),                        \
scheme(sharedScheme),                  \
diffEq( this, & ExplODE<T>::compute ), \
fields( scheme->dimensions(), 0 ),     \
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
            class explode : public sequential<T>
            {
            public:
                //==============================================================
                //
                // types and definitions
                //
                //==============================================================
                typedef vector<T>                      Vector;        //!< alias for internal data
                typedef ODE::ExplicitSolver<T>         SolverType;    //!< alias
                typedef arc_ptr<SolverType>            SolverPointer; //!< alias
                typedef ODE::ExplicitAdjuster<T>       AdjusterType;    //!< alias
                typedef typename AdjusterType::Pointer AdjusterPointer; //!< alias
                typedef ODE::DriverCK<T>               DefaultSolver; //!< default solver

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
                inline explicit explode(const SolverPointer &sharedSolver,
                                        const SchemePointer &sharedScheme) throw() :
                Y_EXPLODE_CTOR(sharedSolver)
                {
                    setup();
                }


                //! setup with a private default solver
                inline explicit ExplODE(const SchemePointer &sharedScheme) throw() :
                Y_EXPLODE_CTOR( DefaultSolver::New() )
                {
                    setup();
                }

                //! access current state
                const accessible<T> & operator*() const throw()
                {
                    return fields;
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(ExplODE);
                SolverPointer                    solver;
                SchemePointer                    scheme;
                typename ODE::Field<T>::Equation diffEq;
                Vector                           fields;
                const accessible<T>             *p_aorg;
                const Variables                 *p_vars;
                T                                __ctrl;

                //! finalize object
                void setup()
                {
                    solver->start( scheme->dimensions() );
                }


                //! ODE wrapper
                inline void compute( array<T> &dYdx, const T x, const array<T> &Y)
                {
                    assert(p_aorg);
                    assert(p_vars);
                    return scheme->rates(dYdx, x, Y, *p_aorg, *p_vars);
                }

                //! initialize protocol
                inline virtual T onStart(const T x1, const accessible<T> &aorg, const Variables &vars)
                {
                    // link
                    p_aorg = &aorg;
                    p_vars = &vars;

                    // setup state and step control
                    scheme->setup(fields,aorg,vars);
                    __ctrl = scheme->delta();

                    // differential step: up to x1
                    (*solver)( diffEq, fields, scheme->start(), x1, __ctrl, scheme->callback() );

                    // done, return the value of interest
                    return scheme->query(x1,fields,aorg,vars);
                }

                //! update protocol
                inline virtual T onReach(const T x1, const accessible<T> &aorg, const Variables &vars)
                {
                    // link
                    p_aorg = &aorg;
                    p_vars = &vars;


                    // differential step
                    (*solver)( diffEq, fields, this->current, x1, __ctrl,  scheme->callback());

                    // done, return the value of interest
                    return scheme->query(x1,fields,aorg,vars);
                }
            };

        }

    }

}

#endif

