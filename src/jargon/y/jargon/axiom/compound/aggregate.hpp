
//! \file

#ifndef Y_JARGON_AXIOM_AGG_INCLUDED
#define Y_JARGON_AXIOM_AGG_INCLUDED 1

#include "y/jargon/axiom/compound.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Aggregate : public Compound
        {
        public:
            static const uint32_t UUID = Y_FOURCC('A', 'G', 'G', 'R');
            virtual ~Aggregate() throw();
            
            
            template <typename LABEL> inline
            explicit Aggregate(const LABEL &id) :
            Compound(id,UUID)
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Aggregate);
            
        };
    }
}
    
#endif
    
