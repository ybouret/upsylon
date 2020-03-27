
//! \file

#ifndef Y_JARGON_XCACHE_INCLUDED
#define Y_JARGON_XCACHE_INCLUDED 1

#include "y/jargon/axiom.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class XCache : public XList
        {
        public:
            explicit XCache(size_t n=0);
            virtual ~XCache() throw();
            const Dogma inactive;
            
            XNode *query();
            XNode *query(const Dogma &, Lexeme *);
            void   store(XNode *xnode) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(XCache);
        };
    }
    
}

#endif
