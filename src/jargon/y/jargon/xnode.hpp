
//! \file

#ifndef Y_JARGON_XNODE_INCLUDED
#define Y_JARGON_XNODE_INCLUDED 1

#include "y/jargon/lexer.hpp"

namespace upsylon {
    
    namespace Jargon {

        //----------------------------------------------------------------------
        //
        //
        //
        //----------------------------------------------------------------------

        class                  Axiom; //! forward declaration
        typedef arc_ptr<Axiom> Dogma; //! shared rule
        
        class                            XNode; //!< Syntax Node
        typedef core::list_of_cpp<XNode> XList; //!< Syntax Node List
        
        //----------------------------------------------------------------------
        //
        //
        //
        //----------------------------------------------------------------------
        class XNode : public Object, public inode<XNode>, public Vizible
        {
        public:
            //! kind of syntax node
            enum Kind
            {
                IsTerminal, //!< a terminal => lexeme
                IsInternal, //!< an internal => children
                IsInactive  //!< memory
            };
                        
            
            static XNode * MakeInactive(const Dogma &);          //!< create an Inactive XNode
            static XNode * NewEffective(const Dogma &,Lexeme *); //!< lexeme==NULL => internal
            
            const Lexeme & lexeme()   const throw(); //!< lexeme for terminal
            XList        & children() throw();       //!< children for internal
            const XList  & children() const throw(); //!< children for internal

            virtual       ~XNode() throw(); //!< cleanup
            static void    Release(XNode *, XList &) throw();
            bool           isInternal() const throw();
            bool           isTerminal() const throw();
            bool           isInactive() const throw();
            
            void           activate(const Dogma &,Lexeme *) throw();
            
            static void Back(XNode    *node, Lexer &, XList &) throw();
            static void Grow(XNode * &xtree, XNode *xnode) throw();
            
            const Kind    kind;
            const Dogma   dogma;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(XNode);
            explicit XNode(const Dogma &) throw(); //!< create an inactive node
            union {
                Lexeme *lexeme;
                char    children[sizeof(XList)];
            } impl;
            void clr() throw();
            virtual void vizCore(ios::ostream &) const;
            void         setDogma(const Dogma &) throw();
            void         setLexeme(Lexeme *)     throw();
        };
        
        
    }
    
}

#endif

