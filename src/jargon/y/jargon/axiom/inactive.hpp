//! \file

#ifndef Y_JARGON_AXIOM_INACTIVE_INCLUDED
#define Y_JARGON_AXIOM_INACTIVE_INCLUDED 1

#include "y/jargon/axiom.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! inactive axiom, for memory
        class Inactive : public Axiom
        {
        public:
            static const uint32_t UUID = Y_FOURCC('D','U','L','L'); //!< UUID
            virtual ~Inactive() throw();
            
            //! setup
            template <typename LABEL> inline
            explicit Inactive(const LABEL &id) : Axiom(id, UUID)
            {
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Inactive);
            virtual const char * vizShape() const throw() { return "oval";    }
            virtual const char * vizStyle() const throw() { return "dotted";  }
        };
        
    }
    
}

#endif

