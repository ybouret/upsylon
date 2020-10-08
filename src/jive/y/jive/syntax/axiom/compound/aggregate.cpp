

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

                const long subDepth = depth+1;
                size_t     refCount = 1;
                for(const Axiom::Reference *ref=head;ref;ref=ref->next,++refCount)
                {
                    Y_JIVE_PRINTLN("|_wait for " << refCount << "/" << size << " of<" << name <<">");
                    const Axiom &axiom = **ref;
                    Node        *sTree = 0;
                    if(axiom.accept(sTree,lexer,source,subDepth))
                    {
                        if(sTree) chld.push_back(sTree);
                        Y_JIVE_PRINTLN("|_accepted " << refCount << "/" << size << " of<" << name <<">");
                    }
                    else
                    {
                        assert(0==sTree);
                        Node::ReturnTo(lexer,node.yield());
                        Y_JIVE_PRINTLN("|_rejected " << refCount << "/" << size << " of<" << name <<">");
                        return false;
                    }
                }
                Y_JIVE_PRINTLN("|_accepting <" << name << "> with #children=" << chld.size);
                if(chld.size>0)
                {
                    Grow(tree,node.yield());
                }
                return true;
            }
        }
    }
}

