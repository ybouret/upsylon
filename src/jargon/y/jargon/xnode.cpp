#include "y/jargon/xnode.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        XNode::~XNode() throw()
        {
            
        }
        
        XNode * XNode:: Create(const Axiom &ax, Lexeme *lx)
        {
            auto_ptr<Lexeme> guard(lx);
            XNode *xnode = new XNode(ax,lx);
            guard.dismiss();
            return xnode;
        }
    }
    
}

#include "y/jargon/axiom.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        bool   XNode:: is(const string &id) const throw()
        {
            return id == *(dogma->label);
        }
        
        bool   XNode:: is(const char *id) const throw()
        {
            return id == *(dogma->label);
        }
        
        template <typename DERIVED>
        Axiom * _Axiom( const DERIVED &derived  ) throw()
        {
            const Axiom &axiom = static_cast<const Axiom &>(derived);
            assert(axiom.refcount()>0);
            return (Axiom *) &axiom;
        }
        
        
        
        
        XNode:: XNode(const Axiom &axiom, Lexeme *lex) throw() :
        dogma(_Axiom(axiom)),
        lexeme(lex),
        children()
        {
            
        }
        
                
        void XNode:: Restore(XNode *xnode, Lexer &lexer) throw()
        {
            assert(xnode);
            assert(0==xnode->next);
            assert(0==xnode->prev);
            
            if( xnode->lexeme.is_valid() )
            {
                lexer.unget(xnode->lexeme.yield());
            }
            
            XList &chld = xnode->children;
            while(chld.size)
            {
                Restore(chld.pop_back(),lexer);
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
                xtree->children.push_back(xnode);
            }
        }
        
        

       
        
    }
    
}

