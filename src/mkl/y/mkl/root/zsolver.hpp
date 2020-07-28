//! \file

#ifndef Y_MKL_ROOT_ZSOLVER_INCLUDED
#define Y_MKL_ROOT_ZSOLVER_INCLUDED

#include "y/mkl/types.hpp"

namespace upsylon {

    namespace mkl {

        class zsolver
        {
        public:
            enum sign_type
            {
                is_negative,
                is_zero,
                is_positive
            };

            virtual ~zsolver() throw();

        protected:
            explicit zsolver() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(zsolver);
        };
    }
}

#endif
