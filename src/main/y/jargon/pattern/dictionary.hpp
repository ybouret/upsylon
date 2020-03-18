//! \file

#ifndef Y_JARGON_PATTERN_DICT_INCLUDED
#define Y_JARGON_PATTERN_DICT_INCLUDED 1

#include "y/jargon/pattern.hpp"
#include "y/associative/suffix-tree.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        typedef suffix_tree<Motif> DictionaryType;
        
        class Dictionary : public DictionaryType
        {
        public:
            
        private:
            
        };
        
    }
    
}

#endif

