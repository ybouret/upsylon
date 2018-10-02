#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {

        void DynamoGenerator:: walkDown( const Node &dynamo )
        {
            if(verbose)
            {
                indent() << std::endl;
                indent() << "walking down {" << parser->name << "}" << std::endl;
                indent() << std::endl;
            }
            
            assert("dynamo"==dynamo.rule.name);
            assert(dynamo.internal);
            for(const Node *node=dynamo.children.head;node;node=node->next)
            {
                ++level;
                assert("RULE"==node->rule.name);
                assert(node->internal);
                assert(node->children.size==2);
                assert(node->children.head->terminal);
                assert(node->children.head->rule.name=="ID");
                const string ruleName = node->children.head->lexeme.to_string();
                if(verbose)
                {
                    indent() << "..building <" << ruleName << ">" << std::endl;
                }
                --level;
            }

            if(verbose)
            {
                indent() << std::endl;
            }
        }

#if 0
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
                        members.add( createFrom(child,hsyn(child->rule.name)) );
                    }
                    --level;
                    break;

                    
                default:
                    members.add( createFrom(parent,parentKind) );
            }
            --level;

        }

        const Syntax::Rule & DynamoGenerator:: createFrom(const Node *node,
                                                          const int   kind )
        {
            const string &name = node->rule.name;
            assert( hsyn(name) == kind );
            if(verbose) { indent() << "createFrom(" << name << ")" << std::endl; }

            switch(kind)
            {


                case isID:
                {
                    assert(node->terminal);
                    const string        id = node->lexeme.to_string();
                    const Syntax::Rule *r  = parser->getRuleByName(id);
                    if(!r)
                    {
                        throw exception("{%s} requires undefined rule '%s'", **(parser->name), *id);
                    }
                    return *r;
                }

                case isZOM:
                {
                    assert(node->internal);
                    assert(1==node->children.size);
                    const Node *child = node->children.head;
                    return parser->zeroOrMore( createFrom(child,hsyn(child->rule.name)) );
                }

                case isOOM:
                {
                    assert(node->internal);
                    assert(1==node->children.size);
                    const Node *child = node->children.head;
                    return parser->oneOrMore( createFrom(child,hsyn(child->rule.name)) );
                }

                case isOPT:
                {
                    assert(node->internal);
                    assert(1==node->children.size);
                    const Node *child = node->children.head;
                    return parser->optional( createFrom(child,hsyn(child->rule.name)) );
                }


                case isAGG:
                {
                    if(verbose) { indent() << "->new AGG" << std::endl; }
                    Syntax::Compound &sub = parser->agg(Syntax::Compound::Design);
                    fill(sub,node);
                    return sub;
                }

                case isALT:
                {
                    if(verbose) { indent() << "->new alt" << std::endl; }
                    Syntax::Compound &sub = parser->alt();
                    fill(sub,node);
                    return sub;
                }

                case isRX:
                case isRS:
                case CARET:
                {
                    int          h  = -1;
                    const string rx = nodeToRegExp(*node,h);
                    switch(h)
                    {
                        case 0: assert("RX"==node->rule.name); break;
                        case 1: assert("RS"==node->rule.name); return parser->sole(rx);

                        case 2: assert("^"==node->rule.name);  break;

                        default:
                            break;
                    }
                }

                default:
                    throw exception("{%s} createFrom('%s') not implemented", **(parser->name), *name );
            }
        }

#endif
        
    }

}
