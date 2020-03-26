
//! \file

#ifndef Y_JARGON_PATTERN_MATCHING_INCLUDED
#define Y_JARGON_PATTERN_MATCHING_INCLUDED 1

#include "y/jargon/pattern.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Dictionary; //!< forward declaration
        
        class Matching : public Motif
        {
        public:
            Matching(const Motif &m) throw();
            Matching(Pattern     *p) throw();
            Matching(const string &rx, const Dictionary *dict=0);
            Matching(const char   *rx, const Dictionary *dict=0);
            Matching(const Matching &) throw();
            virtual ~Matching() throw();
            
            bool   exactly(Token &, const string &);
            bool   partly(Token &, const string &s);
            
        private:
            Y_DISABLE_ASSIGN(Matching);
        };
        
    }
    
}

#endif

