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
#include "quark/v1/neg.hpp"
#include "quark/v1/add.hpp"
#include "quark/v1/sub.hpp"
#include "quark/v1/dot.hpp"
#include "quark/v1/mod2.hpp"
#include "quark/v1/rms.hpp"

#include "quark/v2/def.hpp"
#include "quark/v2/mul.hpp"
#include "quark/v2/mul-trn.hpp"

#include "quark/v3/def.hpp"
#include "quark/v3/mul.hpp"
#include "quark/v3/mul-rtrn.hpp"

        };

    }
}

#endif

