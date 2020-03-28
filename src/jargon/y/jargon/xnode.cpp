

#include "y/jargon/xnode.hpp"
#include "y/type/aliasing.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        XNode::~XNode() throw()
        {
            switch(kind)
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
            clr();
        }
        
        void XNode:: clr() throw()
        {
            aliasing::_(kind) = IsInactive;
            memset(&impl,0,sizeof(impl));
        }
        
        XNode * XNode:: MakeInactive(const Dogma &d)
        {
            return new XNode(d);
        }
        
        bool XNode::isInternal() const throw() { return IsInternal == kind; }
        bool XNode::isTerminal() const throw() { return IsTerminal == kind; }
        bool XNode::isInactive() const throw() { return IsInactive == kind; }
        
        
        XNode * XNode:: NewEffective(const Dogma &hAxiom, Lexeme *lexeme)
        {
            auto_ptr<Lexeme> keep(lexeme);
            XNode           *node = new XNode(hAxiom);
            node->setLexeme(keep.yield());
            return node;
        }
        
        const Lexeme & XNode:: lexeme() const throw()
        {
            assert(IsTerminal==kind);
            assert(impl.lexeme);
            return *(impl.lexeme);
        }
        
        const XList & XNode:: children() const throw()
        {
            assert(IsInternal==kind);
            return *static_cast<const XList *>( aliasing::anonymous(impl.children) );
        }
        
        XList & XNode:: children() throw()
        {
            assert(IsInternal==kind);
            return *static_cast<XList *>( aliasing::anonymous(impl.children) );
        }
        
    }
    
}

#include "y/jargon/axiom.hpp"


namespace upsylon {
    
    namespace Jargon {
        
        XNode:: XNode(const Dogma &d) throw() :
        kind(IsInactive),
        dogma(d),
        impl()
        {
            clr();
        }
        
        
        void XNode:: Release(XNode *xnode, XList &xcache) throw()
        {
            assert(xnode);
            switch(xnode->kind)
            {
                case IsInactive: goto KEEP;
                case IsTerminal: delete xnode->impl.lexeme; break;
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
            xnode->clr();
        KEEP:
            xcache.push_back(xnode);
        }
        
        void XNode:: Back(XNode *xnode, Lexer &lexer, XList &xcache) throw()
        {
            assert(xnode);
            switch(xnode->kind)
            {
                case IsInactive: goto KEEP;
                case IsTerminal: lexer.unget(xnode->impl.lexeme);
                    break;
                case IsInternal: {
                    XList &chld = xnode->children();
                    while(chld.size)
                    {
                        Back(chld.pop_back(),lexer, xcache);
                    }
                } break;
            }
            xnode->clr();
        KEEP:
            xcache.push_back(xnode);
            
        }
        
        void XNode:: setDogma(const Dogma &hAxiom) throw()
        {
            aliasing::_(dogma) = hAxiom;
        }
        
        void XNode:: setLexeme(Lexeme *lexeme) throw()
        {
            if(lexeme)
            {
                impl.lexeme       = lexeme;
                aliasing::_(kind) = IsTerminal;
            }
            else
            {
                aliasing::_(kind) = IsInternal;
                new( & children() ) XList();
            }
        }
        
        void  XNode:: activate(const Dogma &hAxiom, Lexeme *lexeme) throw()
        {
            assert(isInactive());
            assert(0==impl.lexeme);
            setDogma(hAxiom);
            setLexeme(lexeme);
        }
        
    }
    
}

