
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
            static const char     CLID[];                           //!< "Terminal"
            
            //! semantic feature
            enum Feature
            {
                Standard, //!< different values, arising from a regular expression
                Univocal, //!< only one meaningfull value
                Division  //!< syntax only
            };
            
            
            
            //! named feature
            static const char *FeatureText(const Feature) throw();
            
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            virtual ~Terminal() throw(); //!< cleanup
            
            //! setup, check valid name
            template <typename LABEL>
            explicit Terminal(const LABEL  &id,
                              const Feature ft = Standard) :
            Axiom(id,UUID), feature(ft)
            {
                setup();
            }
            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            const char *featureText() const throw(); //!< FeatureText(feature)
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const Feature feature;    //!< the feature
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            virtual const char *   vizual() const throw();
            virtual bool           accept(Y_JARGON_AXIOM_ARGS) const;
            virtual ios::ostream & display(ios::ostream &)     const;

            void setup();
        };
        
    }
    
}

#endif

