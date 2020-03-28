
//! \file

#ifndef Y_JARGON_XCACHE_INCLUDED
#define Y_JARGON_XCACHE_INCLUDED 1

#include "y/jargon/axiom.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class XCache : public XList
        {
        public:
            template <typename LABEL> inline
            explicit XCache( const LABEL &id, size_t n =0) :
            XList(), inactive( new Inactive(id) )
            {
                reserve(n);
            }
            
            
            virtual ~XCache() throw();
            const Dogma inactive;
            
            XNode *query();                       //!< an inactive node
            XNode *query(const Dogma &, Lexeme *);//!< a live node
            void   store(XNode *xnode) throw();   //!< release inside this
            
            void   reserve(size_t n);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(XCache);
        };
    }
    
}

#endif
