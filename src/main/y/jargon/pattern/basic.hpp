
//! \file
#ifndef Y_JARGON_PATTERN_BASIC_INCLUDED
#define Y_JARGON_PATTERN_BASIC_INCLUDED 1

#include "y/jargon/pattern.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! Basic pattern, accepting or not 1 char
        class Basic : public Pattern
        {
        public:
            virtual ~Basic() throw(); //!< cleanup
            
        protected:
            explicit Basic(const uint32_t) throw(); //!< forward uuid
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Basic);
            virtual bool good(const uint8_t)        const throw() = 0;
            virtual bool match(Token &, Source &)   const; //!< if good!
            virtual bool feeble() const throw(); //!< false
        };
        
    }
    
}


#endif

