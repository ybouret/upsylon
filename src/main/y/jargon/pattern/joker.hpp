
//! \file
#ifndef Y_JARGON_PATTERN_JOKER_INCLUDED
#define Y_JARGON_PATTERN_JOKER_INCLUDED 1

#include "y/jargon/pattern.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! Logical operations based pattern
        class Joker : public Pattern
        {
        public:
            virtual ~Joker() throw();            //!< cleanup
                                                 
            const Motif motif;
            
        protected:
            explicit Joker(const uint32_t, const Motif &) throw(); //!< forward uuid
            Joker(const Joker &);                                  //!< full copy
            
            static const char feeble_pattern[]; //!< for exception strings
        private:
            Y_DISABLE_ASSIGN(Joker);
        };
        
    }
    
}


#endif
