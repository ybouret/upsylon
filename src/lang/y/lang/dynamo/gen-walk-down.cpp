#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {
        
        void DynamoGenerator:: walkDown( const Node *node )
        {
            assert(node);
            if(node->terminal)
            {
                if(verbose) { indent() << "@term " << node->rule.name << std::endl; }
            }
            else
            {
                if(verbose) { indent() << "@walk " << node->rule.name << std::endl; }
                ++level;
                if("RULE"==node->rule.name)
                {
                    //__________________________________________________________
                    //
                    // found a top rule
                    //__________________________________________________________
                    const string name = getNodeName(*node,"ID", 0);
                    if(verbose) { indent() << "|_" << name << std::endl;}
                    Syntax::Rule *top = (Syntax::Rule *)(parser->getRuleByName(name));
                    if(!top)
                    {
                        throw exception("{%s} unregistered rule '%s'", **(parser->name), *name );
                    }

                    //__________________________________________________________
                    //
                    // get the compound rule that was created
                    //__________________________________________________________
                    assert( Syntax::Aggregate::UUID == top->uuid || Syntax::Alternate::UUID == top->uuid );
                    assert( NULL != top->data );
                    Syntax::Compound &members = *(Syntax::Compound *)(top->data);
                    fill(members,node->children.tail);

                }
                else
                {
                    for(const Node *child = node->children.head; child; child=child->next )
                    {
                        walkDown(child);
                    }
                }
                --level;
            }

        }


        void DynamoGenerator:: fill( Syntax::Compound &members, const Node *node )
        {
            assert(node!=NULL);
            const string name = node->rule.name;
            if(verbose) { indent() << " |_" << name << std::endl; }
            const int    kind = hsyn(name);
            switch(kind)
            {

                default:
                    throw exception("{%s} rule '%s' not handled", **(parser->name), *name);
            }
        }

    }

}
