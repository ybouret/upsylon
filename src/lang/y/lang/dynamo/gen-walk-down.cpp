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
                case IS_OOM:
                case IS_ZOM:
                case IS_OPT:
                    //__________________________________________________________
                    //
                    // already created: parse children
                    //__________________________________________________________
                    assert(parent->internal);
                    ++level;
                    for(const Node *child=parent->children.head;child;child=child->next)
                    {
                        content.add( createRule(child,hsyn(child->rule.name)) );
                    }
                    --level;
                    break;


                case IS_ID:
                    content.add( getRuleID(fn,parent) );
                    break;

                default:
                    throw Exception(fn,"{%s} unhandled creation of '%s'", **(parser->name), *parentName);
            }
            --level;
        }

        const Syntax::Rule & DynamoGenerator:: createRule(const Node   *node,
                                                          const int     h)
        {
            static const char fn[] = "createRule";
            if(verbose) { indent() << "\\_" << fn << "(" << node->rule.name << ")" << std::endl; }
            switch( h )
            {
                case IS_ID: return getRuleID(fn,node);

                case IS_ALT:
                {
                    //----------------------------------------------------------
                    // a new alternation
                    //----------------------------------------------------------
                    Syntax::Compound &sub = parser->alt();
                    if(verbose) { indent() << "  |_ALT '" << sub.name << "'" << std::endl; }
                    fill(sub,node);
                    return sub;
                }

                case IS_AGG:
                {
                    //----------------------------------------------------------
                    // a new aggregate
                    //----------------------------------------------------------
                    Syntax::Compound &sub = parser->agg();
                    if(verbose) { indent() << "  |_ALT '" << sub.name << "'" << std::endl; }
                    fill(sub,node);
                    return sub;
                }

                case IS_RS:
                {
                    //----------------------------------------------------------
                    // a new(?) string
                    //----------------------------------------------------------
                    assert(node->terminal);
                    const string rs = node->lexeme.to_string(1,1);
                    return getRuleFromString(rs,false);
                }

                case IS_OS:
                {
                    //----------------------------------------------------------
                    // a new(?) string operator
                    //----------------------------------------------------------
                    break;
                }

#define Y_DYNGEN_CREATE(LABEL,METHOD) case IS_##LABEL: do {\
assert(node->internal);\
assert(1==node->children.size);\
assert(0!=node->children.head);\
return parser->METHOD(createRule(node->children.head, hsyn( node->children.head->rule.name ) ) ); } while(false)

                    Y_DYNGEN_CREATE(OOM,oneOrMore);
                    Y_DYNGEN_CREATE(OPT,optional);
                    Y_DYNGEN_CREATE(ZOM,zeroOrMore);


                default:
                    break;
            }
            throw Exception(fn,"createRule(%s) Not implemented", *node->rule.name);
        }

        const Syntax::Rule & DynamoGenerator:: getRuleFromString( const string &rs, const bool isOp )
        {
            static const char fn[] = "getRuleFrom";
            if(verbose) { indent() << "  |_internal string '" << rs << "'" << std::endl; }
            const Terminal *ppTerm = terminals.search(rs);
            if(ppTerm)
            {
                if(verbose) { indent() << "  |_already declared!" << std::endl; }
                const Syntax::Rule &rule = (**ppTerm).rule;
                // TODO: operator promotion
                return rule;
            }
            else
            {
                const string &id = rs;
                const string  rx = StringToRegExp(rs);
                if(verbose) { indent() << "  |_creating from \"" << rx << "\"" << std::endl; }
                const Syntax::Rule &rule = isOp ? parser->op(id,rx) : parser->sole(id,rx);
                const Terminal sym = new _Terminal(rs,rule,parser->name,false);
                if(!terminals.insert(sym))
                {
                    throw Exception(fn,"{%s} unexpected failed registration of '%s'", **(parser->name), *rs);
                }
                return rule;
            }
        }

    }

}
