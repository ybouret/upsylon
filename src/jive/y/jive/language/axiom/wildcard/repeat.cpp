

#include "y/jive/language/axiom/wildcard/repeat.hpp"
#include "y/momentary/increase.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {
            Repeat:: ~Repeat() throw()
            {
            }

            string Repeat:: Mark(const size_t n)
            {
                switch(n)
                {
                    case 0: return string('*');
                    case 1: return string('+');
                    default:
                        break;
                }
                return vformat(">=%lu",(unsigned long) n);
            }

            Y_LANG_AXIOM_IMPL(Repeat)
            {
                Y_LANG_PRINTLN( obs.indent() << "Repeat <" << axiom.name << "> >= " << atLeast);
                size_t count = 0;
                xTree  branch( Node::Acquire(*this) );

                {
                    Observer::Increase scoped(obs.depth);
                    Node::List        &leaves = branch->leaves();
                    {
                        Node *node = 0;
                        while( axiom.accept( node, source, lexer, obs) )
                        {
                            if(!node)
                            {
                                throw exception("invalid repeated <%s>", **(axiom.name) );
                            }
                            leaves.push( node );
                            ++count;
                        }
                    }
                }

                if(count>=atLeast)
                {
                    Y_LANG_PRINTLN( obs.indent() << "Repeat <" << axiom.name << "> >= " << atLeast << " [" << Accepted << " count=" << count << "]");
                    Node::Grow(tree,branch.yield());
                    return true;
                }
                else
                {
                    Y_LANG_PRINTLN( obs.indent() << "Repeat <" << axiom.name << "> >= " << atLeast << " [" << Rejected << " count=" << count << "]");
                    Node::ReturnTo(lexer,branch.yield());
                    return false;
                }
            }

        }

    }

}

