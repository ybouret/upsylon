
//! \file
#ifndef Y_JARGON_PATTERN_BASIC_INCLUDED
#define Y_JARGON_PATTERN_BASIC_INCLUDED 1

#include "y/jargon/pattern.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Basic : public Pattern
        {
        public:
            virtual ~Basic() throw();
            
        protected:
            explicit Basic(const uint32_t) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Basic);
            virtual bool good(const uint8_t ch) const throw() = 0;
        };
        
    }
    
}


#endif

