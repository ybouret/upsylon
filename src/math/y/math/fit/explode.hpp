//! \file
#ifndef Y_MATH_FIT_EXPLODE_INCLUDED
#define Y_MATH_FIT_EXPLODE_INCLUDED 1

#include "y/math/fit/samples.hpp"
#include "y/math/ode/explicit/driver-ck.hpp"

namespace upsylon
{
    namespace math
    {

        namespace ODE
        {
            //! differential system interface
            template <typename T>
            class System : public counted_object
            {
            public:
                typedef arc_ptr< System<T> >          Pointer;   //!< alias
                typedef typename Field<T>::Callback   Callback;  //!< alias
                typedef typename Fit::Type<T>::Array  Array;     //!< alias
                typedef Fit::Variables                Variables; //!< alias

                virtual size_t    dimension() const throw()                 = 0;    //!< dimensionality
                virtual void      setup( array<T> & ) const throw()         = 0;    //!< initialize internal variables at initial coordinate
                virtual T         start() const throw()                     = 0;    //!< return linear=starting point
                virtual T         query( const array<T> &Y )                = 0;    //!< extract scalar data from state
                virtual void      rates(Array &dYdx, T x, const Array &Y,           //|
                                     const Array &aorg, const Variables &vars) = 0; //!< differential rates
                virtual T         delta() const throw() = 0;                        //!< initial time step
                virtual Callback *adapt() throw() { return NULL; }                  //!< callback to correct internal phase space
                inline virtual   ~System() throw() {}                               //!< destructor

            protected:
                inline explicit System() throw() {} //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(System);
            };
        }

        namespace Fit
        {
            //! macro to construct ExplODE
#define Y_MATH_FIT_EXPLODE_CTOR(ARG)   \
ESP(ARG),                              \
sys(sys_),                             \
ode( this, & ExplODE<T>::compute ),    \
arr( sys.dimension() ),                \
p_aorg(0),                             \
p_vars(0),                             \
ctrl(0)

            //! wrapper to provide sequential call
            template <typename T>
            class ExplODE : public ODE::ExplicitSolver<T>::Pointer, public Type<T>::Sequential
            {
            public:
                typedef typename ODE::Field<T>::Equation          Equation;  //!< alias
                typedef typename ODE::ExplicitSolver<T>::Pointer  ESP;       //!< alias
                typedef typename Type<T>::Array                   Array;     //!< alias

                //! initialize internal state
                inline explicit ExplODE(const ESP      &esp_,
                                        ODE::System<T> &sys_) :
                Y_MATH_FIT_EXPLODE_CTOR(esp_)
                {
                    finalize();
                }

                //! default setup
                inline explicit ExplODE(ODE::System<T>         &sys_,
                                        ODE::ExplicitSolver<T> *esp_ = NULL) :
                Y_MATH_FIT_EXPLODE_CTOR(esp_ ? esp_ : ODE::DriverCK<T>::New())
                {
                    finalize();
                }


                //! access to current state
                inline const array<T> &fields() const throw() { return arr; }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(ExplODE);
                ODE::System<T>         &sys;
                Equation                ode;
                vector<T>               arr;
                const Array            *p_aorg;
                const Variables        *p_vars;
                T                       ctrl;

                //! finalize construct
                inline void finalize() { (**this).start( sys.dimension() ); }

                //! ODE wrapper
                inline void compute( Array &dYdx, T x, const Array &Y)
                {
                    assert(p_aorg);
                    assert(p_vars);
                    return sys.rates(dYdx, x, Y, *p_aorg, *p_vars);
                }

                //! initialize protocol
                inline virtual T on_initialize(T x1, const Array &aorg, const Variables &vars)
                {
                    // link
                    p_aorg = &aorg;
                    p_vars = &vars;

                    // setup state and step
                    sys.setup(arr);
                    ctrl = sys.delta();

                    // differential step up to x1
                    (**this)( ode, arr, sys.start(), x1, ctrl, sys.adapt() );
                    
                    // done
                    return sys.query(arr);
                }

                //! update protocol
                inline virtual T on_compute_to(T x1, const Array &aorg, const Variables &vars)
                {
                    // link
                    p_aorg = &aorg;
                    p_vars = &vars;
                    

                    // differential step
                    (**this)( ode, arr, this->current, x1, ctrl,  sys.adapt());
                    
                    // done
                    return sys.query(arr);
                }
            };


        }
    }
}

#endif

