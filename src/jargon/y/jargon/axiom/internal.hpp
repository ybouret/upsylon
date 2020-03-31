
//! \file

#ifndef Y_JARGON_AXIOM_INTERNAL_INCLUDED
#define Y_JARGON_AXIOM_INTERNAL_INCLUDED 1

#include "y/jargon/axiom.hpp"
namespace upsylon {
    
    namespace Jargon {
        
        //! base class for internal axiom
        class Internal : public Axiom
        {
        public:
            
            virtual ~Internal() throw(); //!< cleanup
            template <typename LABEL> inline
            explicit Internal(const LABEL &id, const uint32_t u) : Axiom(id,u)
            {
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Internal);
            virtual const char * vizShape() const throw() { return "house";  }
            virtual const char * vizStyle() const throw() { return "solid";  }
            virtual bool         accept(Y_JARGON_AXIOM_ARGS) const
            {
                (void) xtree;
                (void) lexer;
                (void) source;
                (void) xcache;
                return false;
            }
        };
        
    }
    
}

#endif

