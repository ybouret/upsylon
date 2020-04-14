

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
        
        void  Evaluator:: onOperator(const Tag   &name,
                                     const Token &data,
                                     const size_t argc)
        {
            indent( std::cerr << "|-"  ) << "[call] " << name << "/" << argc;
            if(data.size) std::cerr << ' ' << '<' << data << '>';
            std::cerr << std::endl;
        }

        
        void Evaluator:: browse(const XNode &root)
        {
            depth = 0;
            onInit();
            __browse(root);
            assert(0==depth);
            onQuit();
        }
        
        void Evaluator:: __browse(const XList &children)
        {
            ++depth;
            for(const XNode *child=children.head;child;child=child->next)
            {
                __browse(*child);
            }
            --depth; assert(depth>=0);
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
                    __browse(children);
                    onInternal(xnode.dogma->label,children.size);
                    return;
                }
                    
                case Operator::UUID: {
                    assert(xnode.lexeme.is_valid());
                    const XList &children = xnode.children;
                    __browse(children);
                    onOperator(xnode.dogma->label,*xnode.lexeme,children.size);
                    return;
                }
                    
                default:
                    throw exception("unhandled %s",fourcc_(axiom.uuid));
            }
            
            
            
        }
     
        
        Evaluator_:: ~Evaluator_() throw()
        {
        }
        
        void Evaluator_:: onInit() throw()
        {
        }
        
        void Evaluator_:: onQuit() throw()
        {
            
        }

    }
    
}

