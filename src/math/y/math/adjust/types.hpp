
//! \file

#ifndef Y_MATH_ADJUST_TYPES_INCLUDED
#define Y_MATH_ADJUST_TYPES_INCLUDED 1

#include "y/math/types.hpp"
#include "y/sequence/vector.hpp"
#include "y/ptr/arr.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {

    namespace Adjust {

        typedef vector<size_t> Indices;
        
        template <typename T>
        struct Type
        {
            typedef sequence<T>               Sequence;
            typedef arr_ptr<Sequence,arc_ptr> Series;
            typedef addressable<T>            Array;
        };

    }

}


#endif

