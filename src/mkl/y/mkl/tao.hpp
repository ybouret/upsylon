//! \file

#ifndef Y_MKL_TAO_INCLUDED
#define Y_MKL_TAO_INCLUDED

#include "y/mkl/types.hpp"
#include "y/type/auto-cast.hpp"
#include "y/container/matrix.hpp"

namespace upsylon
{
    
    namespace mkl
    {
        //! optimized argument conversion
#define Y_TAO_TO(TYPE,SOURCE,VALUE) (auto_cast<TYPE,typename SOURCE::mutable_type>::_(VALUE))

        //! optimized argument conversion
#define Y_TAO_CAST(TARGET,SOURCE,VALUE) (auto_cast<typename TARGET::mutable_type,typename SOURCE::mutable_type>::_(VALUE))

        struct tao
        {
#include "tao/v1.hpp"
#include "tao/v2.hpp"
#include "tao/v3.hpp"

        };

    }
}

#endif
