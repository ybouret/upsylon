
//! \file

#ifndef Y_JARGON_AXIOM_OPTIONAL_INCLUDED
#define Y_JARGON_AXIOM_OPTIONAL_INCLUDED 1

#include "y/jargon/axiom/wildcard.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        class Option : public Wildcard
        {
        public:
            static const char     Mark = '?';
            static const uint32_t UUID = Y_FOURCC(Mark,'O','P','T');
            virtual ~Option() throw();
            
            explicit Option(Axiom &);
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Option);
            
        };
    }
    
}

#endif
