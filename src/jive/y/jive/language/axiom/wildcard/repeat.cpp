

#include "y/jive/language/axiom/wildcard/repeat.hpp"

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
                Y_LANG_PRINTLN(obs.indent() << "|_" << name << " (>=" << atLeast << ")");
                XTree       branch( Node::Acquire(*this) );
                Node::List &leaves = branch->leaves();
                {
                    const Observer::Scope keep(obs);
                    while(true)
                    {
                        Node *node = 0;
                        if(axiom.accept(node,source,lexer,obs))
                        {
                            if(!node) throw exception("%s found invalid repeating empty <%s> ", *(obs.gname), **(axiom.name) );
                            leaves.push( node );
                        }
                        else
                        {
                            assert(NULL==node);
                            break;
                        }
                    }
                }


                const size_t count = leaves.size;
                if(count>=atLeast)
                {
                    Node::Grow(tree,branch.yield());
                    Y_LANG_PRINTLN(obs.indent() << "|_" << name << " [" << Accepted << " (" << count << ">=" << atLeast << ")]");
                    return true;
                }
                else
                {
                    Node::ReturnTo(lexer,branch.yield());
                    Y_LANG_PRINTLN(obs.indent() << "|_" << name << " [" << Rejected << " (" << count << "<" << atLeast << ")]");
                    return false;
                }
            }

        }

    }

}

