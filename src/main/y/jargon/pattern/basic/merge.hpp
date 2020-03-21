//! \file

#ifndef Y_JARGON_PATTERN_BASIC_MERGE_INCLUDED
#define Y_JARGON_PATTERN_BASIC_MERGE_INCLUDED 1

#include "y/jargon/pattern/basic/all.hpp"

namespace upsylon {
    
    namespace Jargon {
     
        struct Merge {
            static Pattern * Try(const Pattern *lhs, const Pattern *rhs);
        };
        
    }
    
}

#endif

