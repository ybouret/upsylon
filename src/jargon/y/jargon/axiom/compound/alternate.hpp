

//! \file

#ifndef Y_JARGON_AXIOM_ALT_INCLUDED
#define Y_JARGON_AXIOM_ALT_INCLUDED 1

#include "y/jargon/axiom/compound.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Alternate : public Compound
        {
        public:
            static const uint32_t UUID = Y_FOURCC('A', 'L', 'T', 'R');
            virtual ~Alternate() throw();
            
            
            template <typename LABEL> inline
            explicit Alternate(const LABEL &id) :
            Compound(id,UUID)
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Alternate);
            virtual bool accept(Y_JARGON_AXIOM_ARGS) const;

        };
    }
}

#endif

