
//! \file

#ifndef Y_JARGON_XCACHE_INCLUDED
#define Y_JARGON_XCACHE_INCLUDED 1

#include "y/jargon/axiom/inactive.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! cache of syntax xnode for tree construction
        class XCache : public XList
        {
        public:
            virtual ~XCache() throw(); //!< cleanup
            void reserve( size_t n);   //!< reserve inactive nodes
            
            //! setup
            template <typename LABEL> inline
            explicit XCache(const LABEL &id, size_t n=0 ) :
            dull( new Inactive(id) )
            {
                reserve(n);
            }
            
            
            XNode *query(); 
            XNode *query(const Internal &);
            XNode *query(const Terminal &, Lexeme *lexeme);
            void   store(XNode *xnode) throw();

            //! shared inactive axiom
            const arc_ptr<Inactive> dull;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(XCache);
        };
    }
    
}

#endif
