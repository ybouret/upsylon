

#include "y/jive/syntax/axiom/compound/aggregate.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Aggregate:: ~Aggregate() throw()
            {
            }

            Y_JIVE_AXIOM_ACCEPT_IMPL(Aggregate)
            {
                Y_JIVE_PRINTLN("Aggregate <" << name << ">");
                Node::Pointer node( Node::Acquire(*this) );
                Node::List   &chld = node->leaves();

                for(const Axiom::Reference *ref=head;ref;ref=ref->next)
                {
                    const Axiom &axiom = **ref;
                    Node        *sTree = 0;
                    if(axiom.accept(sTree,lexer,source))
                    {
                        if(sTree) chld.push_back(sTree);
                    }
                    else
                    {
                        assert(0==sTree);
                        Node::ReturnTo(lexer,node.yield());
                    }
                }

                if(chld.size>0)
                {
                    Grow(tree,node.yield());
                }

                return true;
            }
        }
    }
}

