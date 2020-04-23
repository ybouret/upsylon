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
        
        
        bool XNode:: query(string &data,const size_t nskip,const size_t ntrim) const
        {
            data.clear();
            if( lexeme.is_valid() )
            {
                const Char::List &self = *lexeme;
                const size_t      ncut = nskip + ntrim;
                if(ncut>=self.size)
                {
                    data.clear();
                }
                else
                {
                    const Char *ch  = self.head;
                    for(size_t i=nskip;i>0;--i)                       ch=ch->next;
                    for(size_t i=self.size-ncut;i>0;--i,ch=ch->next)  data += char(ch->code);
                }
                return true;
            }
            else
            {
                return false;
            }
        }

    }
    
}

#include "y/jargon/axiom.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        const string & XNode:: name() const throw()
        {
            return *(dogma->label);
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

