
//! \file

#ifndef Y_JARGON_AXIOM_OPTIONAL_INCLUDED
#define Y_JARGON_AXIOM_OPTIONAL_INCLUDED 1

#include "y/jargon/axiom/wildcard.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        class Optional : public Wildcard
        {
        public:
            static const uint32_t UUID = Y_FOURCC('?','O','P','T');
            virtual ~Optional() throw();
            
            explicit Optional(Axiom &axiom);
            
            
        protected:
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Optional);
            
        };
    }
    
}

#endif
