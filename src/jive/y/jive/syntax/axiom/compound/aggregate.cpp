

#include "y/jive/syntax/axiom/compound/aggregate.hpp"
#include "y/momentary/value.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Aggregate:: ~Aggregate() throw()
            {
            }

            bool Aggregate:: persistent() const throw()
            {
                switch (type)
                {
                    case Standard:
                    case Variadic:
                        return true;

                    default:
                        break;
                }
                return false;
            }

            bool Aggregate:: transitory() const throw()
            {
                switch (type)
                {
                    case Grouping:
                        return true;

                    default:
                        break;
                }
                return false;
            }



            Y_JIVE_AXIOM_ACCEPT_IMPL(Aggregate)
            {
                Y_JIVE_PRINTLN("aggregate#" << size << ">");
                typedef momentary_value<Observer::Holder> Temporary;
                
                //--------------------------------------------------------------
                //
                // initialize scanning of children
                //
                //--------------------------------------------------------------
                Node::Pointer   node( Node::Acquire(*this) );
                Node::List   &  chld = node->leaves();
                const long      deep = depth+2;
                size_t          iref = 1;

                //--------------------------------------------------------------
                //
                // update observer
                //
                //--------------------------------------------------------------
                Observer::Holder holder = this;
                if( transitory() )
                {
                    holder = guess.tested;
                }


                //--------------------------------------------------------------
                //
                // start scanning
                //
                //--------------------------------------------------------------
                ++depth;
                for(const Axiom::Reference *ref=head;ref;ref=ref->next,++iref)
                {
                    Y_JIVE_PRINTLN(iref << "/" << size);
                    const Axiom          &axiom = **ref;
                    Node                 *sTree = 0;
                    const Temporary       gKeep(guess.tested,holder);
                    
                    if(axiom.accept(sTree,lexer,source,guess,deep))
                    {
                        if(sTree) chld.push_back(sTree);
                        Y_JIVE_PRINTLN(iref << "/" << size << " -> accepted");
                    }
                    else
                    {
                        assert(0==sTree);
                        Y_JIVE_PRINTLN(iref << "/" << size << " => rejected");
                        Node::ReturnTo(lexer,node.yield());
                        return false;
                    }
                }
                --depth;
                //--------------------------------------------------------------
                //
                // end scanning
                //
                //--------------------------------------------------------------

                Y_JIVE_PRINTLN("=> accepted #" << chld.size);
                if(chld.size>0)
                {
                    Grow(tree,node.yield());
                }
                return true;
            }

            
        }
    }
}

