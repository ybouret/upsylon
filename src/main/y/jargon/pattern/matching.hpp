
//! \file

#ifndef Y_JARGON_PATTERN_MATCHING_INCLUDED
#define Y_JARGON_PATTERN_MATCHING_INCLUDED 1

#include "y/jargon/pattern.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Dictionary;
        
        class Matching : public Token, public Motif
        {
        public:
            Matching(const Cache &, const Motif &m) throw();
            Matching(const Cache &, Pattern     *p) throw();
            Matching(const Cache &, const string &rx, const Dictionary *dict=0);
            Matching(const Cache &, const char   *rx, const Dictionary *dict=0);
            virtual ~Matching() throw();
            
          
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Matching);
        };
        
    }
    
}

#endif

