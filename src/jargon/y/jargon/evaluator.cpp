

#include "y/jargon/evaluator.hpp"
#include "y/jargon/axiom.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Evaluator:: ~Evaluator() throw()
        {
        }
        
        std::ostream & Evaluator:: indent(std::ostream &os) const
        {
            int i = depth;
            while(i-- > 0) os << '-' << '-';
            return os;
        }

       
        void Evaluator:: onTerminal(const Tag   &name,
                                    const Token &token)
        {
            indent( std::cerr << "|-" ) << "[push] " << name;
            if(token.size)
            {
                std::cerr << " = '" << token << "'";
            }
            std::cerr << std::endl;
        }
        
        
        void Evaluator:: onInternal(const Tag    &name,
                                    const size_t  argc)
        {
            indent( std::cerr << "|-"  ) << "[call] " << name << "/" << argc << std::endl;
        }
        
        void Evaluator:: browse(const XNode &root)
        {
            depth = 0;
            onBrowsing();
            __browse(root);
            assert(0==depth);
            onFinalize();
        }
        
        void Evaluator:: __browse(const XNode &xnode)
        {
            switch(xnode.genre)
            {
                case XNode::IsTerminal:
                    assert(xnode.lexeme.is_valid());
                    onTerminal(xnode.dogma->label,*xnode.lexeme); return;
                case XNode::IsInternal: {
                    const XList &children = xnode.children;
                    ++depth;
                    for(const XNode *child=children.head;child;child=child->next)
                    {
                        __browse(*child);
                    }
                    --depth; assert(depth>=0);
                    onInternal(xnode.dogma->label,children.size);
                } return;
            }
        }
     
        
        Evaluator_:: ~Evaluator_() throw()
        {
        }
        
        void Evaluator_:: onBrowsing() throw()
        {
        }
        
        void Evaluator_:: onFinalize() throw()
        {
            
        }

    }
    
}

