

#include "y/jargon/xnode.hpp"
#include "y/type/aliasing.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        XNode::~XNode() throw()
        {
            switch(genre)
            {
                case IsInactive: //std::cerr << "inactive" << std::endl;
                    return;
                    
                case IsTerminal: //std::cerr << "terminal" << std::endl;
                    delete & lexeme();
                    break;
                    
                case IsInternal: //std::cerr << "internal "<< children().size << std::endl;
                    children().release();
                    break;
            }
            shutdown();
        }
        
        void XNode:: cleanup() throw()
        {
            memset(&depot,0,sizeof(depot));
        }
        
        
        void XNode:: shutdown() throw()
        {
            cleanup();
            aliasing::_(genre) = IsInactive;
        }
        
      
        
        bool XNode::isInternal() const throw() { return IsInternal == genre; }
        bool XNode::isTerminal() const throw() { return IsTerminal == genre; }
        bool XNode::isInactive() const throw() { return IsInactive == genre; }
        
        
        
        const Lexeme & XNode:: lexeme() const throw()
        {
            assert(isTerminal());
            assert(depot.lexeme);
            return *(depot.lexeme);
        }
        
        const XList & XNode:: children() const throw()
        {
            assert(isInternal());
            return *static_cast<const XList *>( aliasing::anonymous(depot.children) );
        }
        
        XList & XNode:: children() throw()
        {
            assert(isInternal());
            return *static_cast<XList *>( aliasing::anonymous(depot.children) );
        }
        
        XNode * XNode:: Create(const Inactive &dogma)
        {
            return new XNode(dogma);
        }
        
        
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
        
        
        XNode:: XNode(const Inactive &axiom) throw():
        genre(IsInactive),
        dogma( DerivedToAxiom(axiom) ),
        depot()
        {
            cleanup();
        }
        
        XNode:: XNode(const Terminal &axiom, Lexeme *lexeme) throw() :
        genre(IsTerminal),
        dogma(DerivedToAxiom(axiom)),
        depot()
        {
            cleanup();
            depot.lexeme = lexeme;
        }
        
        XNode:: XNode(const Internal &axiom) throw():
        genre(IsInternal),
        dogma(DerivedToAxiom(axiom)),
        depot()
        {
            cleanup();
            new ( & children() ) XList();
        }
        
        XNode *XNode:: activate(const Internal &axiom) throw()
        {
            assert(isInactive());
            const Dogma target = DerivedToAxiom(axiom);
            aliasing::_(dogma) = target;
            aliasing::_(genre) = IsInternal;
            new( & children() ) XList();
            return this;
        }

        XNode *XNode:: activate(const Terminal &axiom, Lexeme *lexeme) throw()
        {
            assert(isInactive());
            assert(lexeme);
            
            const Dogma target = DerivedToAxiom(axiom);
            aliasing::_(dogma) = target;
            aliasing::_(genre) = IsTerminal;
            depot.lexeme       = lexeme;
            
            return this;
        }

        
        void XNode:: Release(XNode *xnode, XList &xcache) throw()
        {
            assert(xnode);
            switch(xnode->genre)
            {
                case IsInactive: goto KEEP;
                case IsTerminal: delete xnode->depot.lexeme; break;
                case IsInternal:
                {
                    XList &chld = xnode->children();
                    while(chld.size)
                    {
                        XNode *xn = chld.pop_back();
                        Release(xn,xcache);
                    }
                } break;
            }
            xnode->shutdown();
        KEEP:
            xcache.push_back(xnode);
        }
        
        void XNode:: Restore(XNode *xnode, Lexer &lexer, XList &xcache) throw()
        {
            assert(xnode);
            switch(xnode->genre)
            {
                case IsInactive: goto KEEP;
                case IsTerminal: lexer.unget(xnode->depot.lexeme);
                    break;
                case IsInternal: {
                    XList &chld = xnode->children();
                    while(chld.size)
                    {
                        Restore(chld.pop_back(),lexer,xcache);
                    }
                } break;
            }
            xnode->shutdown();
        KEEP:
            xcache.push_back(xnode);
            
        }
        
        void XNode:: Advance(XNode * &tree, XNode *node) throw()
        {
            assert(node);
            if(NULL==tree)
            {
                tree = node;
            }
            else
            {
                assert(tree->isInternal());
                tree->children().push_back(node);
            }
        }

        const Lexeme * XNode:: LastLexeme(const XNode *xnode) throw()
        {
            assert(xnode);
            switch(xnode->genre)
            {
                case IsInternal:
                {
                    const XList &chld = xnode->children();
                    if(chld.size>0)
                    {
                        return LastLexeme(chld.tail);
                    }
                    else
                    {
                        return NULL;
                    }
                }
                    
                case IsTerminal:  return & (xnode->lexeme());
                    
                default:
                    break;
            }
            return NULL;
        }

       
        
    }
    
}

