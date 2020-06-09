//! \file
#ifndef Y_MKL_ODE_IMPLICIT_STEP_INCLUDED
#define Y_MKL_ODE_IMPLICIT_STEP_INCLUDED 1

#include "y/mkl/ode/types.hpp"

namespace upsylon {

    namespace mkl {
        
        namespace ODE {
            
            //! implicit step definition
            template <typename T>
            class ImplicitStep : public Field<T>::Arrays
            {
            public:
                typedef typename Field<T>::Equation  equation; //!< equation alias
                typedef typename Field<T>::Jacobian  jacobian; //!< jacobian alias
                
                //! destructor
                virtual ~ImplicitStep() throw();
                
                //! step API
                virtual void operator()(array<T>       &y,
                                        array<T>       &dydx,
                                        T              &x,
                                        T               htry,
                                        T               eps,
                                        array<T>       &yscal,
                                        T              &hdid,
                                        T              &hnext,
                                        equation       &derivs,
                                        jacobian       &jacobn
                                        ) = 0;
            protected:
                //! constructor for num local arrays
                explicit ImplicitStep( const size_t num );
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(ImplicitStep);
            };
            
        }
        
    }
}



#endif
