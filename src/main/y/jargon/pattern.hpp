//! \file

#ifndef Y_JARGON_PATTERN_INCLUDED
#define Y_JARGON_PATTERN_INCLUDED 1

#include "y/jargon/source.hpp"

namespace upsylon {
 
    namespace Jargon {
    
        class Pattern : public CountedObject
        {
        public:
            
            virtual ~Pattern() throw();
            
            virtual Pattern * clone() const = 0;
            
            
            const uint32_t uuid;
            void          *self;
            
        protected:
            explicit Pattern(const uint32_t) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pattern);
        };
        
        typedef arc_ptr<const Pattern> Motif;
        
#define Y_PATTERN_SELF() self = static_cast<Pattern *>(this)
        
    }
    
}

#endif

