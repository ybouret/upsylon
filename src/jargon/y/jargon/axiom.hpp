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
        
        //! for verbosity
#define Y_JAXIOM(CODE) do { if(Axiom::Verbose) { CODE; } } while(false)
        
        //----------------------------------------------------------------------
        //
        //! base class for a syntax axiom
        //
        //----------------------------------------------------------------------
        class Axiom : public  CountedObject
        {
        public:
            static bool Verbose; //!< global verbosity flag
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
            
            //------------------------------------------------------------------
            //
            // helpers
            //
            //------------------------------------------------------------------
            
            //! recall derived class
            template <typename AXIOM> inline
            AXIOM & as( ) throw()
            {
                assert(AXIOM::UUID==uuid);
                assert(self);
                return *static_cast<AXIOM*>(self);
            }
            
            //! recall const derived class
            template <typename AXIOM> inline
            const AXIOM & as( ) const throw()
            {
                assert(AXIOM::UUID==uuid);
                assert(self);
                return *static_cast<const AXIOM*>(self);
            }
            
            std::ostream & display( std::ostream &os, const char *id ) const;
            
        protected:
            void           *self; //!< pointer to derived class
            
            //! setup for derived classes
            template <typename LABEL> inline
            Axiom( const LABEL &id, const uint32_t u) :
            label( Tags::Make(id) ),
            uuid(u),
            self(0)
            {
                std::cerr << "+Axiom<" << label << "> [" << fourcc_(uuid) << "]" << std::endl;
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Axiom);
            
            
        };
        
       

       
        
    }
}

#endif

