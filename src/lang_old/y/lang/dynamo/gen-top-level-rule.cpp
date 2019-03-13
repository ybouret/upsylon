#include "y/lang/dynamo/generator.hpp"

namespace upsylon
{
    namespace Lang
    {
        void DynamoGenerator:: onRule( const Node &rule )
        {
            static const char fn[] = "Rule";

            //------------------------------------------------------------------
            //
            // get the rule name
            //
            //------------------------------------------------------------------

            Y_DYNAMO_CHECK(rule.internal);
            Y_DYNAMO_CHECK(rule.rule.name=="RULE");
            const Node::List &children = rule.children;
            Y_DYNAMO_CHECK(children.size==2||children.size==3);

            Y_DYNAMO_CHECK(children.head);
            Y_DYNAMO_CHECK(children.head->terminal);
            Y_DYNAMO_CHECK(children.head->rule.name=="ID");
            const string ruleName = rule.children.head->lexeme.to_string();

            //------------------------------------------------------------------
            //
            // get the rule kind
            //
            //------------------------------------------------------------------
            Y_DYNAMO_CHECK(children.tail);
            Y_DYNAMO_CHECK(children.tail!=children.head);
            const string ruleKind = rule.children.tail->rule.name;

            bool setDesign = false;
            if(3==children.size)
            {
                const Node *info = children.head->next;
                Y_DYNAMO_CHECK(info->terminal);
                Y_DYNAMO_CHECK(info->rule.name=='!');
                setDesign = true;
            }

            //------------------------------------------------------------------
            //
            // create the top-level compound
            //
            //------------------------------------------------------------------
            if(verbose) {
                indent() << "--> Rule <" << ruleName << ">/" << ruleKind << std::endl;
                if(setDesign)
                {
                    indent() << "  \\_marked as design rule" << std::endl;
                }
            }

            Syntax::Compound *global = 0;
            const bool first = (parser->top() == 0);
            const int  rcode = hsyn(ruleKind);
            switch( rcode )
            {
                case IS_AGG:
                    if(setDesign)
                    {
                        global = & parser->design(ruleName);
                    }
                    else
                    {
                        global = & parser->agg(ruleName);
                    }
                    break;

                case IS_ALT:
                    if(setDesign)
                    {
                        throw Exception(fn,"Alternate '%s' marked as design", *ruleName );
                    }
                    (void)  parser->alt(ruleName);
                    break;

                case IS_OOM:
                case IS_ZOM:
                case IS_OPT:
                case IS_ID: // not efficient...
                    if(setDesign)
                    {
                        throw Exception(fn,"Acting-like '%s' marked as design", *ruleName );
                    }
                    global = & parser->acting(ruleName);
                    if(!first) global=0;
                    break;

                default:
                    throw Exception(fn,"{%s} unhandled rule kind '%s'", **(parser->name), *ruleKind );
            }


            if(global)
            {
                Y_DYNAMO_CHECK(modules.size()>0);
                const Internal s = new _Internal( *global, modules.peek() );
                if(!internals.insert(s))
                {
                    throw Exception(fn,"{%s} unexpected registration of <%s>", **(parser->name), *ruleName);
                }
            }

        }

    }

}


