

#include "y/jargon/xnode.hpp"
#include "y/type/aliasing.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        XNode::~XNode() throw()
        {
            
        }
        
        
        
      
        
        bool XNode::isInternal() const throw() { return IsInternal == genre; }
        bool XNode::isTerminal() const throw() { return IsTerminal == genre; }
        
        
        
        
        
        XNode * XNode:: Create(const Internal &dogma)
        {
            return new XNode(dogma);
        }
        
       
        
    }
    
}

#include "y/jargon/axiom/all.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        XNode * XNode:: Create(const Terminal &dogma, Lexeme *lexeme)
        {
            assert(lexeme);
            try
            {
                if( *(lexeme->label) != *(dogma.label) )
                {
                    throw exception("Jargon::Terminal '%s'!='%s' !!", **(dogma.label), **(lexeme->label) );
                }
                return new XNode(dogma,lexeme);
            }
            catch(...)
            {
                delete lexeme;
                throw;
            }
        }
        
        template <typename DERIVED>
        Axiom * DerivedToAxiom( const DERIVED &derived  ) throw()
        {
            const Axiom &axiom = static_cast<const Axiom &>(derived);
            assert(axiom.refcount()>0);
            return (Axiom *) &axiom;
        }
        
        
        
        
        XNode:: XNode(const Terminal &axiom, Lexeme *lex) throw() :
        genre(IsTerminal),
        dogma(DerivedToAxiom(axiom)),
        lexeme(lex),
        children()
        {
            
        }
        
        XNode:: XNode(const Internal &axiom) throw():
        genre(IsInternal),
        dogma(DerivedToAxiom(axiom)),
        lexeme(0),
        children()
        {
            
        }
        
        
        
        void XNode:: Restore(XNode *xnode, Lexer &lexer) throw()
        {
            assert(xnode);
            assert(0==xnode->next);
            assert(0==xnode->prev);
            switch(xnode->genre)
            {
                case IsTerminal:
                    assert(xnode->lexeme.is_valid());
                    lexer.unget(xnode->lexeme.yield());
                    break;
                    
                case IsInternal: {
                    XList &chld = xnode->children;
                    while(chld.size)
                    {
                        Restore(chld.pop_back(),lexer);
                    }
                } break;
            }
            delete xnode;
        }
        
        void XNode:: Advance(XNode * &xtree, XNode *xnode) throw()
        {
            assert(xnode);
            if(NULL==xtree)
            {
                xtree = xnode;
            }
            else
            {
                assert(xtree->isInternal());
                xtree->children.push_back(xnode);
            }
        }
        
        void XNode:: Combine(XNode * &xtree, XNode *xnode) throw()
        {
            assert(xnode);
            assert(0==xnode->next);
            assert(0==xnode->prev);
            if(NULL==xtree)
            {
                xtree = xnode; //!< shouldn't happen
            }
            else
            {
                assert(xtree->isInternal());
                switch(xnode->genre)
                {
                    case IsTerminal:
                        xtree->children.push_back(xnode);
                        break;
                        
                    case IsInternal:
                        xtree->children.merge_back( xnode->children );
                        delete xnode;
                        break;
                }
            }
        }
        
        


       
        
    }
    
}

