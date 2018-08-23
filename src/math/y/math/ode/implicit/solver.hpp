#ifndef YOCTO_STIFF_SOLVER_INCLUDED
#define YOCTO_STIFF_SOLVER_INCLUDED 1

#include "y/math/ode/implicit/step.hpp"
#include "y/math/ode/solver-data.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            
            template <typename T>
            class ImplicitSolver : public SolverData<T>
            {
            public:
                typedef typename ImplicitStep<T>::equation equation;
                typedef typename ImplicitStep<T>::jacobian jacobian;
                
                explicit ImplicitSolver();
                virtual ~ImplicitSolver() throw();
                
                void operator()(equation         &derivs,
                                jacobian         &jacobn,
								ImplicitStep<T>  &forward,
								array<T>         &ystart,
								const T           x1,
								const T           x2,
								T                &h1
								);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(ImplicitSolver);
            };
            
            template <
            typename T,
            template <class> class STEP
            > class ImplicitDriver : public ImplicitSolver<T>
            {
            public:
                typedef typename ImplicitStep<T>::equation equation;
                typedef typename ImplicitStep<T>::jacobian jacobian;

                
                explicit ImplicitDriver() : ImplicitSolver<T>(), step_() {}
                virtual ~ImplicitDriver() throw() {}
                
                inline void start( size_t nv )
                {
                    assert(nv>0);
                    this->acquire( nv );  //-- solver data
                    step_.prepare( nv );  //-- step data
                }
                
                void operator()(equation         &derivs,
                                jacobian         &jacobn,
								array<T>         &ystart,
								const T           x1,
								const T           x2,
								T                &h1
								)
                {
                    ImplicitSolver<T> &s = *this;
                    s(derivs,jacobn,step_,ystart,x1,x2,h1);
                }
                
            private:
                STEP<T> step_;
                Y_DISABLE_COPY_AND_ASSIGN(ImplicitDriver);
            };
            
            
        }
    }
}

#endif
