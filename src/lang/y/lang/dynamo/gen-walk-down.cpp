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
                    if(verbose) { indent() << "|_<" << name << ">" << std::endl;}

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
                    Syntax::Compound &compound = *(Syntax::Compound *)(top->data);
                    assert(node->internal);
                    assert(node->children.size==2);
                    assert(node->children.tail);
                    const Node   *subNode = node->children.tail;
                    fill(compound,subNode);
#if 0
                    switch( subKind )
                    {
                        case isAGG:
                        case isALT:
                            fill(compound,subNode);
                            break;

                        case isZOM:
                        case isOOM:
                        case isOPT:
                        case isID:
                            fill(compound,subNode);
                            break;

                        default:
                            throw exception("{%s} invalid content '%s' for top level rule '%s'", **(parser->name), *name, *subName);
                    }
#endif


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

        void DynamoGenerator:: fill( Syntax::Compound &members, const Node *parent )
        {
            assert(parent!=NULL);
            ++level;
            const string &parentName = parent->rule.name;
            const int     parentKind = hsyn(parentName);
            if(verbose) { indent() << "|_fill from <" << parent->rule.name << ">" << std::endl; }
            switch(parentKind)
            {
                case isALT:
                case isAGG:
                    assert(parent->internal);
                    ++level;
                    for(const Node *child=parent->children.head;child;child=child->next)
                    {
                        members.add( createFrom(child) );
                    }
                    --level;
                    break;

                case isID: {
                    assert(parent->terminal);
                    const string linkName = parent->lexeme.to_string();
                    if(verbose) { indent() << parentName << "->" << linkName << std::endl; }
                    const Syntax::Rule *r = parser->getRuleByName(linkName);
                    if(!r)
                    {
                        throw exception("{%s} in '%s', rule '%s' doesn't exist!!!", **(parser->name), *parentName, *linkName );
                    }
                    members.add(*r);
                } break;

                default:
                    indent() << parentName << " not implemented" << std::endl;
            }
            --level;

        }

        const Syntax::Rule & DynamoGenerator:: createFrom( const Node *node )
        {
            const string &name = node->rule.name;
            std::cerr << "Creating rule from " << name << std::endl;

            throw exception("not implemented");
        }


    }

}
