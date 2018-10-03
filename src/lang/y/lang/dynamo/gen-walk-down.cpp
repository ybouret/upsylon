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


        const Syntax::Rule & DynamoGenerator:: getRuleID(const char *fn, const Node *node) const
        {
            assert(node->terminal);
            const string        id = node->lexeme.to_string();
            const Syntax::Rule *r  = parser->getRuleByName(id);
            if(verbose) { indent() << "  |_ID='" << id << "'" << std::endl; }
            if(!r) throw Exception(fn,"{%s} '%s' was not declared", **(parser->name), *id);
            return *r;
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
                    break;

                case IS_ZOM:
                    assert(parent->internal);
                    assert(1==parent->children.size);
                    content.add( parser->zeroOrMore( createRule(parent->children.head) ) );
                    break;

                case IS_OOM:
                    assert(parent->internal);
                    assert(1==parent->children.size);
                    content.add( parser->oneOrMore( createRule(parent->children.head) ) );
                    break;

                case IS_OPT:
                    assert(parent->internal);
                    assert(1==parent->children.size);
                    content.add( parser->optional( createRule(parent->children.head) ) );
                    break;

                case IS_ID:
                    content.add( getRuleID(fn,parent) );
                    break;

                default:
                    throw Exception(fn,"{%s} unhandled creation of '%s'", **(parser->name), *parentName);
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
                case IS_ID: return getRuleID(fn,node);

                case IS_ALT: {
                    //----------------------------------------------------------
                    // a new alternation
                    //----------------------------------------------------------
                    Syntax::Compound &sub = parser->alt();
                    if(verbose) { indent() << "  |_ALT '" << sub.name << "'" << std::endl; }
                    fill(sub,node);
                    return sub;
                }

                default:
                    break;
            }
            throw Exception(fn,"Not implemented");
        }

        
    }

}
