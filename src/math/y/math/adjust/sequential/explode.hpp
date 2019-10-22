

#ifndef Y_MATH_ADJUST_SEQUENTIAL_EXPLODE_INCLUDED
#define Y_MATH_ADJUST_SEQUENTIAL_EXPLIDE_INCLUDED 1

#include "y/math/adjust/sequential.hpp"
#include "y/math/ode/explicit/scheme.hpp"
#include "y/math/ode/explicit/driver-ck.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            //! ExplicitODE
            template <typename T>
            class ExplODE : public Sequential<T>
            {
            public:
                typedef typename Type<T>::Vector      Vector;
                typedef typename Type<T>::Array       Array;
                typedef typename Type<T>::Parameters  Parameters;

                typedef ODE::ExplicitSolver<T>        SolverType;
                typedef arc_ptr<SolverType>           SolverPointer;

                typedef ODE::ExplicitScheme<T>        SchemeType;
                typedef typename SchemeType::Pointer  SchemePointer;

                inline virtual ~ExplODE() throw()
                {
                    
                }

                inline explicit ExplODE(const SolverPointer &sharedSolver,
                                        const SchemePointer &sharedScheme) throw() :
                solver(sharedSolver),
                scheme(sharedScheme),
                diffEq( this, & ExplODE<T>::compute ),
                fields( scheme->dimensions(), 0 ),
                p_aorg(0),
                p_vars(0),
                __ctrl(0)
                {
                    setup();
                }




            private:
                Y_DISABLE_COPY_AND_ASSIGN(ExplODE);
                SolverPointer                    solver;
                SchemePointer                    scheme;
                typename ODE::Field<T>::Equation diffEq;
                Vector                           fields;
                const Parameters                *p_aorg;
                const Variables                 *p_vars;
                T                                __ctrl;

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
                inline virtual T on_initialize(const T x1, const Parameters &aorg, const Variables &vars)
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
                inline virtual T on_compute_to(const T x1, const Parameters &aorg, const Variables &vars)
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

