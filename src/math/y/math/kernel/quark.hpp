
//! \file
#ifndef Y_MK_QUARK_INCLUDED
#define Y_MK_QUARK_INCLUDED 1

#include "y/math/types.hpp"
#include "y/type/auto-cast.hpp"
#include "y/concurrent/scheme/simd.hpp"
#include "y/container/matrix.hpp"
#include "y/core/loop.hpp"

namespace upsylon {

    namespace math {

#include "quark/v1/def.hpp"


        //! Quick Universal Algebraic Reusable Kernel
        struct quark {

#include "quark/v1/ld.hpp"
#include "quark/v1/set.hpp"
#include "quark/v1/add.hpp"
#include "quark/v1/sub.hpp"
#include "quark/v1/dot.hpp"
#include "quark/v1/mod2.hpp"
            
        };

    }
}

#endif

