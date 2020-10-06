//! \file

#ifndef Y_AQUA_TYPES_INCLUDED
#define Y_AQUA_TYPES_INCLUDED 1

#include "y/container/matrix.hpp"

namespace upsylon {

    namespace Aqua
    {
        typedef ptrdiff_t      Int;     //!< integer type
        typedef matrix<Int>    iMatrix; //!< matrix of integer
        typedef matrix<double> Matrix;  //!< default matrix
        
    }
}

#endif

