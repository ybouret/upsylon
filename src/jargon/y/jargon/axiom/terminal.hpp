
#ifndef Y_JARGON_AXIOM_TERMINAL_INCLUDED
#define Y_JARGON_AXIOM_TERMINAL_INCLUDED 1

#include "y/jargon/axiom.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! terminal
        /**
         accept a lexeme with the same label
         */
        //----------------------------------------------------------------------
        class Terminal : public Axiom
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            static const uint32_t UUID = Y_FOURCC('T','E','R','M'); //!< UUID
            enum Feature
            {
                Standard, //!< different values, arising from a regular expression
                Univocal, //!< only one meaningfull value
                Division  //!< syntax only
            };
            
            //------------------------------------------------------------------
            //
            // C++ and definitions
            //
            //------------------------------------------------------------------
            virtual ~Terminal() throw(); //!< cleanup
            
            //! setup, check valid name
            template <typename LABEL>
            explicit Terminal(const LABEL &id, const Feature f = Standard) :
            Axiom(id,UUID), feature(f)
            {
                setup();
            }
            
            const Feature feature;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            virtual const char *   vizShape() const throw() { return "rectangle"; }
            virtual const char *   vizStyle() const throw() { return "solid";  }
            virtual bool           accept(Y_JARGON_AXIOM_ARGS) const;
            virtual ios::ostream & display(ios::ostream &)     const;

            void setup();
        };
        
    }
    
}

#endif

