
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

#include "quark/def.hpp"


        struct quark {

#include "quark/ld.hpp"
#include "quark/set.hpp"
#include "quark/add.hpp"
#include "quark/sub.hpp"
#include "quark/dot.hpp"
#include "quark/mod2.hpp"
            
        };

    }
}

#endif

