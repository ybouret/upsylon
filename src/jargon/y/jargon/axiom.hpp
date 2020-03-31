//! \file

#ifndef Y_JARGON_AXIOM_INCLUDED
#define Y_JARGON_AXIOM_INCLUDED 1

#include "y/jargon/xnode.hpp"
#include "y/jargon/tags.hpp"
#include "y/type/fourcc.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class XCache;
        
#define Y_JARGON_AXIOM_ARGS \
XNode * &xtree,  \
Lexer   &lexer,  \
Source  &source, \
XCache  &xcache
        


        //! base class for a syntax axiom
        class Axiom : public  CountedObject
        {
        public:
            const Tag      label; //!< shared label
            const uint32_t uuid;  //!< UUID
            
            virtual ~Axiom() throw();
            
            virtual const char * vizShape() const throw() = 0; //!< graphViz shape
            virtual const char * vizStyle() const throw() = 0; //!< graphViz style
            virtual bool         accept(Y_JARGON_AXIOM_ARGS) const = 0; //!< main call
            
        protected:
            //! setup
            template <typename LABEL> inline
            Axiom( const LABEL &id, const uint32_t u) :
            label( Tags::Make(id) ),
            uuid(u)
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Axiom);
        };
        
       
       
       
        
    }
}

#endif

