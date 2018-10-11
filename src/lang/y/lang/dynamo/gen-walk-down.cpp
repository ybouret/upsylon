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
                Y_DYNAMO_CHECK("RULE"==node->rule.name);
                Y_DYNAMO_CHECK(node->internal);
                Y_DYNAMO_CHECK(node->children.size==2);
                Y_DYNAMO_CHECK(node->children.head->terminal);
                Y_DYNAMO_CHECK(node->children.head->rule.name=="ID");
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
                Y_DYNAMO_CHECK(rule->isCompound());
                assert(rule->data);
                Syntax::Compound   &content = *(Syntax::Compound *)(rule->data);

                //______________________________________________________________
                //
                // get the rule parent node
                //______________________________________________________________
                const Syntax::Node *parent  = node->children.tail;
                const int           p_code = hsyn(parent->rule.name);
                switch(p_code)
                {
                        // AGG and ALT are already created => forward
                    case IS_AGG:
                    case IS_ALT:
                        fill(content,parent,p_code);
                        break;

                        // need to fill parent
                    default:
                        content.add( createRule(parent,p_code) );
                        break;
                }


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


        void DynamoGenerator:: fill(Syntax::Compound &content,
                                    const Node       *parent,
                                    const int         p_code)
        {
            static const char fn[] = "fill";
            assert(parser->owns(content));
            assert(parent);
            assert(hsyn(parent->rule.name)==p_code);
            ++level;
            if(verbose) { indent() << "\\_" << fn << "@<" << parent->rule.name << ">" << std::endl; }

            switch(p_code)
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
                    throw Exception(fn,"{%s} unexpected creation of '%s'", **(parser->name), *(parent->rule.name) );
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
                case IS_ID:
                {
                    //----------------------------------------------------------
                    // rule must have been created before
                    //----------------------------------------------------------
                    return getRuleID(fn,node);
                }

                case IS_ALT:
                {
                    //----------------------------------------------------------
                    // a new alternation
                    //----------------------------------------------------------
                    Syntax::Compound &sub = parser->alt();
                    if(verbose) { indent() << "  |_ALT '" << sub.name << "'" << std::endl; }
                    fill(sub,node,hsyn(node->rule.name));
                    return sub;
                }

                case IS_AGG:
                {
                    //----------------------------------------------------------
                    // a new aggregate, temporary, so acting...
                    //----------------------------------------------------------
                    Syntax::Compound &sub = parser->agg(Syntax::Compound::Acting);
                    if(verbose) { indent() << "  |_AGG '" << sub.name << "'" << std::endl; }
                    fill(sub,node,hsyn(node->rule.name));
                    return sub;
                }

                case IS_RS:
                {
                    //----------------------------------------------------------
                    // a new(?) string
                    //----------------------------------------------------------
                    Y_DYNAMO_CHECK(node->terminal);
                    const string rs = node->lexeme.to_string(1,1);
                    return getRuleFromString(rs,false);
                }

                case IS_OS:
                {
                    //----------------------------------------------------------
                    // a new(?) string operator
                    //----------------------------------------------------------
                    Y_DYNAMO_CHECK(node->internal);
                    Y_DYNAMO_CHECK(node->children.size==1);
                    Y_DYNAMO_CHECK(node->children.head);
                    const Node *RS = node->children.head;
                    Y_DYNAMO_CHECK( hsyn(RS->rule.name)==IS_RS );
                    Y_DYNAMO_CHECK(RS->terminal);
                    const string rs = RS->lexeme.to_string(1,1);
                    return getRuleFromString(rs,true);
                }

                case IS_RX:
                {
                    //----------------------------------------------------------
                    // a new(?) regexp terminal
                    //----------------------------------------------------------
                    const string rx = node->lexeme.to_string(1,1);
                    return getRuleFromRegExp(rx);
                }

#define Y_DYNGEN_CREATE(LABEL,METHOD) case IS_##LABEL: do {\
Y_DYNAMO_CHECK(node->internal);        \
Y_DYNAMO_CHECK(1==node->children.size);\
Y_DYNAMO_CHECK(0!=node->children.head);\
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
            static const char fn[] = "getRuleFromString";

            if(verbose) { indent() << "  |_internal string '" << rs << "'" << std::endl; }
            const Terminal *ppTerm = terminals.search(rs);

            if(ppTerm)
            {
                //______________________________________________________________
                //
                // found the same registered raw string
                //______________________________________________________________
                if(verbose) { indent() << "  |_already declared!" << std::endl; }
                const Syntax::Rule &rule = (**ppTerm).rule;
                Y_DYNAMO_CHECK(rule.isTerminal());

                //--------------------------------------------------------------
                // check operator level
                //--------------------------------------------------------------
                if(isOp)
                {
                    assert(rule.data);
                    const Syntax::Terminal *t = static_cast<const Syntax::Terminal *>(rule.data);
                    if(!t->isOperator)
                    {
                        if(verbose) { indent() << "  |_operator promotion!" << std::endl; }
                        (bool&)(t->isOperator) = true;
                    }
                }
                return rule;
            }
            else
            {
                //______________________________________________________________
                //
                // create it
                //______________________________________________________________
                const string &id = rs;
                const string  rx = StringToRegExp(rs);
                if(verbose) { indent() << "  |_creating from \"" << rx << "\"" << std::endl; }
                const Syntax::Rule &rule = isOp ? parser->op(id,rx) : parser->mark(id,rx);
                const Terminal sym = new _Terminal(rs,rule,parser->name,false);
                if(!terminals.insert(sym))
                {
                    throw Exception(fn,"{%s} unexpected failed registration of '%s'", **(parser->name), *rs);
                }
                return rule;
            }
        }

        const Syntax::Rule & DynamoGenerator:: getRuleFromRegExp( const string &rx )
        {
            static const char fn[] = "getRuleFromRegExp";
            if(verbose) { indent() << "  |_internal regexp '" << rx << "'" << std::endl; }
            const Terminal *ppTerm = terminals.search(rx);
            if(ppTerm)
            {
                if(verbose) { indent() << "  |_already declared!" << std::endl; }
                const Syntax::Rule &rule = (**ppTerm).rule;
                return rule;
            }
            else
            {
                const string &id = rx;
                if(verbose) { indent() << "  |_creating from \"" << rx << "\"" << std::endl; }
                const Syntax::Rule &rule = parser->term(id,rx);
                const Terminal      sym  = new _Terminal(rx,rule,parser->name,true);
                if(!terminals.insert(sym))
                {
                    throw Exception(fn,"{%s} unexpected failed registration of '%s'", **(parser->name), *rx);
                }
                return rule;
            }


        }


    }

}
