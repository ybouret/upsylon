//! \file

#ifndef Y_JARGON_GRAMMAR_INCLUDED
#define Y_JARGON_GRAMMAR_INCLUDED 1

#include "y/jargon/xcache.hpp"

namespace upsylon {
    
    namespace Jargon {
     
        
        class Grammar
        {
        public:
            const Tag title;
            
            typedef suffix_tree<Dogma> Axioms;
            
            virtual ~Grammar() throw();
            
            template <typename LABEL>
            inline Grammar( const LABEL &id ) :
            title( Tags::Make(id) ),
            root(NULL),
            axioms()
            {
            }
            
            
            
        private:
            Axiom *root;
            Axioms axioms;
        };
        
    }
}

#endif

