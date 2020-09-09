//! \file

#ifndef Y_MATH_FCN_JACOBIAN_INCLUDED
#define Y_MATH_FCN_JACOBIAN_INCLUDED 1

#include "y/container/matrix.hpp"

namespace upsylon
{
    namespace mkl
    {
        //______________________________________________________________________
        //
        //
        //! jacobian interface
        //
        //______________________________________________________________________
        template <typename T>
        class jacobian
        {
        public:
            //! cleanup
            inline virtual ~jacobian() throw() {}

            //! interface: compute jacobian at X
            virtual void operator()( matrix<T> &J, const accessible<T> &X) = 0;

        protected:
            //! setup
            inline explicit jacobian() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(jacobian);
        };

       
    }
}


#endif

