
//! \file

#ifndef Y_JARGON_XCACHE_INCLUDED
#define Y_JARGON_XCACHE_INCLUDED 1

#include "y/jargon/axiom.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class XCache : public XList
        {
        public:
            virtual ~XCache() throw();
            void reserve( size_t n);
            template <typename LABEL> inline
            explicit XCache(const LABEL &id, size_t n=0 ) :
            dull( new Inactive(id) )
            {
                reserve(n);
            }
            
            const arc_ptr<Inactive> dull;
            
            XNode *query(); 
            XNode *query(const Internal &);
            XNode *query(const Terminal &, Lexeme *lexeme);
            void   store(XNode *xnode) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(XCache);
        };
    }
    
}

#endif
