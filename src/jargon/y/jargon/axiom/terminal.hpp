
#ifndef Y_JARGON_AXIOM_TERMINAL_INCLUDED
#define Y_JARGON_AXIOM_TERMINAL_INCLUDED 1

#include "y/jargon/axiom.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! terminal
        /**
         accept a lexeme with the same label
         */
        class Terminal : public Axiom
        {
        public:
            static const uint32_t UUID = Y_FOURCC('T','E','R','M'); //!< UUID
            
            virtual ~Terminal() throw(); //!< cleanup
            
            //! setup
            template <typename LABEL>
            explicit Terminal(const LABEL &id) :
            Axiom(id,UUID)
            {
                setup();
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            virtual const char * vizShape() const throw() { return "rectangle"; }
            virtual const char * vizStyle() const throw() { return "solid";  }
            virtual bool         accept(Y_JARGON_AXIOM_ARGS) const;
            void setup();
        };
        
    }
    
}

#endif

