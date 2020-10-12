

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
                Y_JIVE_PRINTLN("aggregate#" << size << ">");
                Node::Pointer node( Node::Acquire(*this) );
                Node::List   &chld = node->leaves();

                const long subDepth = depth+2;
                size_t     refCount = 1;
                ++depth;
                for(const Axiom::Reference *ref=head;ref;ref=ref->next,++refCount)
                {
                    Y_JIVE_PRINTLN(refCount << "/" << size);
                    const Axiom &axiom = **ref;
                    Node        *sTree = 0;
                    if(axiom.accept(sTree,lexer,source,mind,subDepth))
                    {
                        if(sTree) chld.push_back(sTree);
                        Y_JIVE_PRINTLN(refCount << "/" << size << " -> accepted");
                    }
                    else
                    {
                        assert(0==sTree);
                        Node::ReturnTo(lexer,node.yield());
                        Y_JIVE_PRINTLN(refCount << "/" << size << " => rejected");
                        return false;
                    }
                }
                --depth;
                Y_JIVE_PRINTLN("=> accepted #" << chld.size);
                if(chld.size>0)
                {
                    Grow(tree,node.yield());
                }
                return true;
            }

            void Aggregate:: expect(TermCheckList &ex, Axiom::Registry &db) const
            {

            }
        }
    }
}

