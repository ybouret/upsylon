#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {

        void DynamoGenerator:: walkDown( const Node &dynamo )
        {
            static const char fn[] = "walkDown";
            if(verbose)
            {
                indent() << std::endl;
                indent() << "walking down {" << parser->name << "}" << std::endl;
                indent() << std::endl;
            }

            assert("dynamo"==dynamo.rule.name);
            assert(dynamo.internal);

            //__________________________________________________________________
            //
            //
            // Loop over top-level rules
            //
            //__________________________________________________________________

            for(const Node *node=dynamo.children.head;node;node=node->next)
            {
                ++level;

                //______________________________________________________________
                //
                // get the rule name
                //______________________________________________________________
                assert("RULE"==node->rule.name);
                assert(node->internal);
                assert(node->children.size==2);
                assert(node->children.head->terminal);
                assert(node->children.head->rule.name=="ID");
                const string ruleName = node->children.head->lexeme.to_string();
                if(verbose)
                {
                    indent() << "--> building <" << ruleName << ">" << std::endl;
                }

                //______________________________________________________________
                //
                // get the rule content
                //______________________________________________________________
                const Rule *rule = parser->getRuleByName(ruleName);
                if(!rule)
                {
                    throw Exception(fn,"{%s} unexpected missing <%s>", **(parser->name), *ruleName);
                }
                assert(Syntax::Aggregate::UUID==rule->uuid||Syntax::Alternate::UUID==rule->uuid);
                assert(rule->data);
                Syntax::Compound &content = *(Syntax::Compound *)(rule->data);
                fill(content,node->children.tail);
                --level;
            }

            if(verbose)
            {
                indent() << std::endl;
            }
        }


    

        void DynamoGenerator:: fill(Syntax::Compound &content, const Node *parent)
        {
            static const char fn[] = "fill";
            assert(parser->owns(content));
            assert(parent);
            ++level;
            const string &parentName = parent->rule.name;
            if(verbose) { indent() << "\\_" << fn << "@<" << parentName << ">" << std::endl; }

            switch( hsyn(parentName) )
            {
                case IS_AGG:
                case IS_ALT:
                    //__________________________________________________________
                    //
                    // already created: parse children
                    //__________________________________________________________
                    assert(parent->internal);
                    ++level;
                    for(const Node *child=parent->children.head;child;child=child->next)
                    {
                        content.add( createRule(child) );
                    }
                    --level;
                default:;
            }
            --level;
        }

        const Syntax::Rule & DynamoGenerator:: createRule(const Node *node)
        {
            static const char fn[] = "createRule";
            assert(node);
            const string &nodeName = node->rule.name;
            if(verbose) { indent() << "\\_" << fn << "(" << nodeName << ")" << std::endl; }
            switch( hsyn(nodeName) )
            {
                case IS_ID: {
                    //----------------------------------------------------------
                    // the rule must exists
                    //----------------------------------------------------------
                    assert(node->terminal);
                    const string        id = node->lexeme.to_string();
                    const Syntax::Rule *r = parser->getRuleByName(id);
                    if(verbose) { indent() << "  |_ID='" << id << "'" << std::endl; }
                    if(!r) throw Exception(fn,"{%s} '%s' was not declared", **(parser->name), *id);
                    return *r;
                } break;

                default:
                    break;
            }
            throw Exception(fn,"Not implemented");
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
