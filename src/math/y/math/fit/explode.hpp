//! \file
#ifndef Y_MATH_FIT_EXPLODE_INCLUDED
#define Y_MATH_FIT_EXPLODE_INCLUDED 1

#include "y/math/fit/samples.hpp"
#include "y/math/ode/types.hpp"

namespace upsylon
{
    namespace math
    {

        namespace ODE
        {
            template <typename T>
            class System
            {
            public:
                typedef typename Field<T>::Callback   Callback; //!< alias
                typedef typename Fit::Type<T>::Array  Array;    //!< alias

                inline virtual ~System() throw();

                virtual size_t dimension() const throw()                 = 0; //!< dimensionality
                virtual void   setup( array<T> & ) const throw()         = 0; //!< initialize internal variables at initial coordinate
                virtual T      start() const throw()                     = 0; //!< return linear=starting point
                virtual T      query( const array<T> &Y )                = 0; //!< extract scalar data from state
                virtual void   rates(Array &dYdx, T x, const Array &Y,                   //|
                                     const Array &aorg, const Fit::Variables &vars) = 0; //!< differential rates
                virtual T      delta() const throw() = 0;

            protected:
                inline explicit System() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(System);
            };
        }

        namespace Fit
        {



            //! wrapper to provide sequential call
            template <typename T>
            class ExplODE : public Type<T>::Sequential
            {
            public:
                typedef typename ODE::Field<T>::Equation Equation;
                typedef typename Type<T>::Array          Array;

                inline explicit ExplODE( ODE::System<T> &s ) :
                sys(s),
                ode( this, & ExplODE<T>::compute ),
                arr( sys.dimension() ),
                p_aorg(0),
                p_vars(0)
                {
                }

                inline const array<T> &fields() const throw() { return arr; }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(ExplODE);
                ODE::System<T>  &sys;
                Equation         ode;
                vector<T>        arr;
                const Array     *p_aorg;
                const Variables *p_vars;

                inline void compute( Array &dYdx, T x, const Array &Y )
                {

                }

                inline virtual T on_initialize(T x, const Array &aorg, const Variables &vars)
                {
                    sys.setup(arr);
                    T xstart = sys.start();
                    T ctrl   = sys.delta();

                    return sys.query(arr);
                }

                inline virtual T on_compute_to(T, const Array &aorg, const Variables &vars)
                {

                }

            public:
                T ctrl;
            };
        }
    }
}

#endif

