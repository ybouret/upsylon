

//! \file

#ifndef Y_JARGON_AXIOM_ALT_INCLUDED
#define Y_JARGON_AXIOM_ALT_INCLUDED 1

#include "y/jargon/axiom/compound.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! Alternate: accept one of the axioms, first in order of appearance
        //
        //----------------------------------------------------------------------
        class Alternate : public Compound
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            static const uint32_t UUID = Y_FOURCC('A', 'L', 'T', 'R'); //!< UUID
            static const char     CLID[];                              //!< "Alternate"

            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! cleanup
            virtual ~Alternate() throw();
            
            //! setup
            explicit Alternate(unsigned &);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Alternate);
            virtual bool           accept(Y_JARGON_AXIOM_ARGS) const;
            virtual const char *   vizual() const throw();
            virtual ios::ostream & display(ios::ostream &)     const;

        };
    }
}

#endif

