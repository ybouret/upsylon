//! \file
#ifndef YOCTO_ODE_ROSENBROCK_INCLUDED
#define YOCTO_ODE_ROSENBROCK_INCLUDED 1

#include "y/math/ode/implicit/step.hpp"
#include "y/math/kernel/lu.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            //! Rosenbrock's methods
            template <typename T>
            class Rosenbrock : public ImplicitStep<T>
            {
            public:
                //! destructor
                virtual ~Rosenbrock() throw();
                //! memory acquisition
                void     prepare( size_t nv );
                
                
            protected:
                explicit Rosenbrock(); //!< constructor
                array<T>  &g1;    //!< internal trial 1
                array<T>  &g2;    //!< internal trial 2
                array<T>  &g3;    //!< internal trial 3
                array<T>  &g4;    //!< internal trial 4
                array<T>  &dfdx;  //!< internal slope
                array<T>  &err;   //!< internal error
                array<T>  &ysav;  //!< internal temp
                array<T>  &dysav; //!< internal temp
                matrix<T>  dfdy;  //!< internal jacobia
                matrix<T>  a;     //!< internal matrix
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Rosenbrock);
            };

                     
        }
        
    }
    
}



#endif
