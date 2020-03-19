//! \file
#ifndef Y_JARGON_PATTERN_LOGIC_INCLUDED
#define Y_JARGON_PATTERN_LOGIC_INCLUDED 1

#include "y/jargon/pattern.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! Logic based pattern
        class Logic : public Pattern
        {
        public:
            virtual ~Logic() throw(); //!< cleanup
            
        protected:
            explicit Logic(const uint32_t) throw(); //!< forward uuid
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Logic);
        
        };
        
    }
    
}


#endif
