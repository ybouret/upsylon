
#ifndef Y_JARGON_AXIOM_TERMINAL_INCLUDED
#define Y_JARGON_AXIOM_TERMINAL_INCLUDED 1

#include "y/jargon/axiom.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Terminal : public Axiom
        {
        public:
            static const uint32_t UUID = Y_FOURCC('T','E','R','M');
            
            virtual ~Terminal() throw();
            
            template <typename LABEL>
            explicit Terminal(const LABEL &id) :
            Axiom(id,UUID)
            {}
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            virtual const char * vizShape() const throw() { return "rectangle"; }
            virtual const char * vizStyle() const throw() { return "solid";  }
        };
        
    }
    
}

#endif

