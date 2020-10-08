


#include "y/jive/syntax/axiom/compound/alternate.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Alternate:: ~Alternate() throw()
            {
            }

            Y_JIVE_AXIOM_ACCEPT_IMPL(Alternate)
            {
                Y_JIVE_PRINTLN(" alternate #" << size);

                const long subDepth = depth+1;
                size_t     refCount=1;
                bool       result  = false;
                for(const Axiom::Reference *ref=head;ref;ref=ref->next,++refCount)
                {
                    const Axiom &axiom = ref->axiom;
                    Y_JIVE_PRINTLN(refCount << "/" << size << " -> <" << axiom.name << ">");
                    Node *sub = 0;
                    if( axiom.accept(sub,lexer,source,subDepth))
                    {
                        result = true;
                        if(NULL==sub)
                        {
                            Y_JIVE_PRINTLN(refCount << "/" << size << " <- [empty]");
                        }
                        else
                        {
                            Y_JIVE_PRINTLN(refCount << "/" << size << " <- <" << sub->axiom.name << ">");
                            Grow(tree,sub);
                            return true;
                        }
                    }
                }
                Y_JIVE_PRINTLN(" accepted [empty]");
                return result;
            }
        }
    }
}

