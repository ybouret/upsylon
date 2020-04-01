//! \file

#ifndef Y_JARGON_AXIOM_WILDCARD_INCLUDED
#define Y_JARGON_AXIOM_WILDCARD_INCLUDED 1

#include "y/jargon/axiom/internal.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        class Wildcard : public Internal
        {
        public:
            const Canon canon;
            
            virtual ~Wildcard() throw();
            
            template <typename LABEL> inline
            explicit Wildcard(const LABEL   &id,
                              const uint32_t u,
                              Axiom         &axiom
                              ) :
            Internal(id,u),
            canon( checkShared(axiom) )
            {
            }
            
            const Axiom & operator*() const throw();
            
        protected:
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Wildcard);
            Axiom * checkShared(Axiom &axiom) const;
            
        };
    }
    
}

#endif
