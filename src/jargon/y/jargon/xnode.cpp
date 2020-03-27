

#include "y/jargon/xnode.hpp"
#include "y/type/aliasing.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        XNode::~XNode() throw()
        {
            std::cerr << "~XNode()" << std::endl;
            switch(kind)
            {
                case IsInactive: std::cerr << "inactive" << std::endl;
                    return;
                    
                case IsTerminal: std::cerr << "terminal" << std::endl;
                    delete & lexeme();
                    break;
                    
                case IsInternal: std::cerr << "internal "<< children().size << std::endl;
                    while( children().size )
                    {
                        delete children().pop_back();
                    }
                    children().release();
                    //assert(0==children().size);
                    break;
            }
            clr();
        }
        
        void XNode:: clr() throw()
        {
            aliasing::_(kind) = IsInactive;
            memset(&impl,0,sizeof(impl));
        }
        
        XNode:: XNode() throw() :
        kind(IsInactive),
        impl()
        {
            //std::cerr << "sizeof(imlp)=" << sizeof(impl) << std::endl;
            clr();
        }
        
        XNode * XNode:: Create()
        {
            return new XNode();
        }
        
        bool XNode::isInternal() const throw() { return IsInternal == kind; }
        bool XNode::isTerminal() const throw() { return IsTerminal == kind; }
        bool XNode::isInactive() const throw() { return IsInactive == kind; }

        
        XNode * XNode:: Create(Lexeme *lexeme)
        {
            auto_ptr<Lexeme> keep(lexeme);
            XNode           *node = new XNode();
            node->inactiveTo(keep.yield());
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


        
        void XNode:: Release(XNode *xnode, XList &xcache) throw()
        {
            assert(xnode);
            switch(xnode->kind)
            {
                case IsInactive: return;
                case IsTerminal: delete & (xnode->lexeme()); break;
                case IsInternal:
                {
                    XList &chld = xnode->children();
                    while(chld.size)
                    {
                        XNode *xn = chld.pop_back();
                        Release(xn,xcache);
                        xcache.push_back(xn);
                    }
                } break;
            }
            xnode->clr();
        }
        
        void  XNode:: inactiveTo(Lexeme *lexeme) throw()
        {
            assert(isInactive());
            impl.lexeme       = lexeme;
            aliasing::_(kind) =  (NULL!=lexeme) ? IsTerminal : IsInternal;
        }

        
        void XNode:: vizCore( ios::ostream &fp ) const
        {
            
        }
    }
}
