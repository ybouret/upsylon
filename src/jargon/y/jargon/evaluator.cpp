

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
        
        void Evaluator:: __browse(const XNode &node)
        {
            switch(node.genre)
            {
                case XNode::IsInactive: return;
                case XNode::IsTerminal: onTerminal(node.dogma->label,node.lexeme()); return;
                case XNode::IsInternal: {
                    const XList &children = node.children();
                    ++depth;
                    for(const XNode *child=children.head;child;child=child->next)
                    {
                        __browse(*child);
                    }
                    --depth; assert(depth>=0);
                    onInternal(node.dogma->label,children.size);
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

