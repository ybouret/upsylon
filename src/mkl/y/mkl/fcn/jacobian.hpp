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
            inline virtual ~jacobian() throw()
            {
            }

            virtual void operator()( matrix<T> &J, const accessible<T> &X) = 0;

        protected:
            inline explicit jacobian() throw()
            {
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(jacobian);
        };

       
    }
}


#endif

