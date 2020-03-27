//! \file

#ifndef Y_JARGON_AXIOM_INCLUDED
#define Y_JARGON_AXIOM_INCLUDED 1

#include "y/jargon/tags.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Axiom : public  Object
        {
        public:
            const Tag label;
            
            virtual ~Axiom() throw();
            
        protected:
            template <typename LABEL> inline
            Axiom( const LABEL &id ) :
            label( Tags::Make(id) )
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Axiom);
        };
    }
}

#endif

