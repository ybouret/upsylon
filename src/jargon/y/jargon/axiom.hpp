//! \file

#ifndef Y_JARGON_AXIOM_INCLUDED
#define Y_JARGON_AXIOM_INCLUDED 1

#include "y/jargon/xnode.hpp"
#include "y/jargon/tags.hpp"
#include "y/type/fourcc.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //
        // Helpers
        //
        //
        //----------------------------------------------------------------------

        class XCache; //! forward declaration
        
        //! accept method arguments
#define Y_JARGON_AXIOM_ARGS \
XNode * &xtree,             \
Lexer   &lexer,             \
Source  &source,            \
XCache  &xcache
        

        //----------------------------------------------------------------------
        //
        //! base class for a syntax axiom
        //
        //----------------------------------------------------------------------
        class Axiom : public  CountedObject
        {
        public:
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            virtual ~Axiom() throw(); //!< cleanup
            
            //------------------------------------------------------------------
            //
            // virtual interface
            //
            //------------------------------------------------------------------
            virtual const char * vizShape() const throw()          = 0; //!< graphViz shape
            virtual const char * vizStyle() const throw()          = 0; //!< graphViz style
            virtual bool         accept(Y_JARGON_AXIOM_ARGS) const = 0; //!< main call
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const Tag      label; //!< shared label
            const uint32_t uuid;  //!< UUID for I/O
            
        protected:
            //! setup for derived classes
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

