//! \file

#ifndef Y_JARGON_AXIOM_INCLUDED
#define Y_JARGON_AXIOM_INCLUDED 1

#include "y/jargon/xnode.hpp"
#include "y/jargon/tags.hpp"
#include "y/type/fourcc.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        template <typename AXIOM>
        class NodeOf : public Object, public inode< NodeOf<AXIOM> >
        {
        public:
            //! setup
            inline explicit NodeOf(const AXIOM &args) throw() : axiom(args) {}
            
            //! cleanup
            inline virtual ~NodeOf() throw() {}
            
            const AXIOM &axiom; //!< reference to AXIOM

        private:
            Y_DISABLE_COPY_AND_ASSIGN(NodeOf);
        };
        
        
        
        typedef NodeOf<Terminal>            TermNode;
        typedef core::list_of_cpp<TermNode> TermList;
        
        class AlphaNode : public Object, public inode<AlphaNode>
        {
        public:
            //! setup
            explicit AlphaNode(const Axiom &args) throw() :
            axiom(args) {}
            virtual ~AlphaNode() throw() {}
            
            const Axiom &axiom;
            TermList     terms;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(AlphaNode);
        };
        
        typedef core::list_of_cpp<AlphaNode> AlphaList;
        
        //----------------------------------------------------------------------
        //
        //
        // Helpers
        //
        //
        //----------------------------------------------------------------------
        class XCache; //!< forward declaration
        
        
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
            virtual const char *   vizual() const throw()            = 0; //!< graphViz shape+style
            virtual bool           accept(Y_JARGON_AXIOM_ARGS) const = 0; //!< main call
            virtual ios::ostream & display(ios::ostream &)     const = 0; //!< display
            
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
            
            bool isTerminal() const throw(); //!< terminal
           
            //! join to manifest first apparent from *this
            bool joinFirstApparentTo(AlphaList &) const;
            
            //! join to the list the first terminal from *this
            bool joinFirstTerminalTo(TermList &) const;
            
        protected:
            void           *self; //!< pointer to derived class
            
            //! setup for derived classes
            template <typename LABEL> inline
            Axiom( const LABEL &id, const uint32_t u) :
            label( Tags::Make(id) ),
            uuid(u),
            self(0)
            {
            }
            
            //! check label for derived class, not beginning by reserved chars
            void checkLabel(const char *which) const;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Axiom);
            
            
        };
        
       

       
        
    }
}

#endif

