
//! \file

#ifndef Y_JARGON_AXIOM_OPTIONAL_INCLUDED
#define Y_JARGON_AXIOM_OPTIONAL_INCLUDED 1

#include "y/jargon/axiom/wildcard.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! an optional axiom
        //
        //----------------------------------------------------------------------
        class Option : public Wildcard
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            static const char     Mark = '?';                        //!< label marker
            static const uint32_t UUID = Y_FOURCC(Mark,'O','P','T'); //!< UUID
            static const char     CLID[];                            //!< "Option"
            
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            virtual ~Option() throw(); //!< cleanup
            explicit Option(Axiom &);  //!< setup
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Option);
            virtual bool           accept(Y_JARGON_AXIOM_ARGS) const;
            virtual ios::ostream & display(ios::ostream &)     const;
            virtual const char    *vizual() const throw();
        };
    }
    
}

#endif
