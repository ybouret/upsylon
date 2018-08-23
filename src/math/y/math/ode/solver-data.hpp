//! \file
#ifndef Y_MATH_ODE_SOLVER_DATA_INCLUDED
#define Y_MATH_ODE_SOLVER_DATA_INCLUDED 1

#include "y/math/ode/types.hpp"

namespace upsylon
{

    namespace math
    {
        namespace ODE
        {

            //! common data for solvers
            template <typename T>
            class SolverData : public Field<T>::Arrays
            {
            public:
                //! destructor
                inline virtual ~SolverData() throw() {}

                T hmin; //!< initial=0
                T eps;  //!< initial=sqrt_ftol
                T TINY; //!< initial=1e-30, for yscal

                inline explicit SolverData() :
                Field<T>::Arrays( 3 ),
                hmin( 0 ),
                eps(  numeric<T>::ftol ),
                TINY( T(1e-30) ),
                y(     this->next() ),
                dydx(  this->next() ),
                yscal( this->next() )
                {
                }

            protected:
                array<T> &y;     //!< current y
                array<T> &dydx;  //!< current slope
                array<T> &yscal; //!< current scaling


            private:
                Y_DISABLE_COPY_AND_ASSIGN(SolverData);
            };

        }

    }
}


#endif


