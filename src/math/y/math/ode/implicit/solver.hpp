//! \file
#ifndef Y_STIFF_SOLVER_INCLUDED
#define Y_STIFF_SOLVER_INCLUDED 1

#include "y/math/ode/implicit/step.hpp"
#include "y/math/ode/solver-data.hpp"

namespace upsylon
{
    namespace math
    {
        namespace ODE
        {
            
            //! solver interface
            template <typename T>
            class ImplicitSolver : public SolverData<T>
            {
            public:
                typedef typename ImplicitStep<T>::equation equation; //!< equation alias
                typedef typename ImplicitStep<T>::jacobian jacobian; //!< jacobian alias
                
                //! constructor
                explicit ImplicitSolver();
                //! destructor
                virtual ~ImplicitSolver() throw();
                
                //! solve interface
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
            
            //! drive an implicit step
            template <
            typename T,
            template <class> class STEP
            > class ImplicitDriver : public ImplicitSolver<T>
            {
            public:
                typedef typename ImplicitStep<T>::equation equation; //!< equation alias
                typedef typename ImplicitStep<T>::jacobian jacobian; //!< jacobian alias

                //! constructor
                explicit ImplicitDriver() : ImplicitSolver<T>(), step_() {}
                //! destructor
                virtual ~ImplicitDriver() throw() {}
                
                //! acquire all memory
                inline void start( size_t nv )
                {
                    assert(nv>0);
                    this->acquire( nv );  //-- solver data
                    step_.prepare( nv );  //-- step data
                }
                
                //! perform the step
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
