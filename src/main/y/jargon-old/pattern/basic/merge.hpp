//! \file

#ifndef Y_JARGON_PATTERN_BASIC_MERGE_INCLUDED
#define Y_JARGON_PATTERN_BASIC_MERGE_INCLUDED 1

#include "y/jargon/pattern/basic/all.hpp"

namespace upsylon {
    
    namespace Jargon {
     
        //! Merge functions
        struct Merge {
            //! try to merge two patterns
            static Pattern * Try(const Pattern *lhs, const Pattern *rhs);
        };
        
    }
    
}

#endif

