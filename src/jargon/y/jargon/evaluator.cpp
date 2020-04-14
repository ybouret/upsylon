

#include "y/jargon/evaluator.hpp"
#include "y/jargon/axiom/terminal.hpp"
#include "y/jargon/axiom/operator.hpp"
#include "y/jargon/axiom/compound/aggregate.hpp"
#include "y/exception.hpp"

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
            indent( std::cerr << "|-"  ) << "[make] " << name << "/" << argc << std::endl;
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
            const Axiom &axiom = *(xnode.dogma);
            switch(axiom.uuid)
            {
                case Terminal::UUID:
                    assert(xnode.lexeme.is_valid());
                    onTerminal(axiom.label, *xnode.lexeme);
                    return;
                    
                case Aggregate::UUID: {
                    const XList &children = xnode.children;
                    ++depth;
                    for(const XNode *child=children.head;child;child=child->next)
                    {
                        __browse(*child);
                    }
                    --depth; assert(depth>=0);
                    onInternal(xnode.dogma->label,children.size);
                    return;
                }
                    
                default:
                    throw exception("unhandled %s",fourcc_(axiom.uuid));
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

