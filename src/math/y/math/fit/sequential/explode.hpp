

#ifndef Y_MATH_ADJUST_SEQUENTIAL_EXPLODE_INCLUDED
#define Y_MATH_ADJUST_SEQUENTIAL_EXPLIDE_INCLUDED 1

#include "y/math/fit/sequential.hpp"
#include "y/math/ode/explicit/scheme.hpp"
#include "y/math/ode/explicit/driver-ck.hpp"

namespace upsylon {

    namespace math {

        namespace Fit {

            //! constructor for ExplODE
#define Y_EXPLODE_CTOR(SOLVER) \
Sequential<T>(),\
solver(SOLVER),\
scheme(sharedScheme),\
diffEq( this, & ExplODE<T>::compute ),\
fields( scheme->dimensions(), 0 ),\
p_aorg(0),\
p_vars(0),\
__ctrl(0)

            //==================================================================
            //
            //
            //! Explicit ODE, sequential integration
            //
            //
            //==================================================================
            template <typename T>
            class ExplODE : public Sequential<T>
            {
            public:
                //==============================================================
                //
                // types and definitions
                //
                //==============================================================
                typedef typename Type<T>::Vector      Vector;        //!< alias for internal data
                typedef ODE::ExplicitSolver<T>        SolverType;    //!< alias
                typedef arc_ptr<SolverType>           SolverPointer; //!< alias
                typedef ODE::ExplicitScheme<T>        SchemeType;    //!< alias
                typedef typename SchemeType::Pointer  SchemePointer; //!< alia


                //==============================================================
                //
                //  members
                //
                //==================================================================

                //! cleanup
                inline virtual ~ExplODE() throw()
                {
                    
                }

                //! setup, preparing solver
                inline explicit ExplODE(const SolverPointer &sharedSolver,
                                        const SchemePointer &sharedScheme) throw() :
                Y_EXPLODE_CTOR(sharedSolver)
                {
                    setup();
                }


                //! setup with a private solver
                inline explicit ExplODE(const SchemePointer &sharedScheme) throw() :
                Y_EXPLODE_CTOR(ODE::DriverCK<T>::New())
                {
                    setup();
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

                    // setup state and step
                    scheme->setup(fields,aorg,vars);
                    __ctrl = scheme->delta();

                    // differential step up to x1
                    (*solver)( diffEq, fields, scheme->start(), x1, __ctrl, scheme->callback() );

                    // done
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

                    // done
                    return scheme->query(x1,fields,aorg,vars);
                }
            };

        }

    }

}

#endif

