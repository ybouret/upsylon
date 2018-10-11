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
            Y_DYNAMO_CHECK(rule.children.size==2);
            Y_DYNAMO_CHECK(rule.children.head);
            Y_DYNAMO_CHECK(rule.children.head->terminal);
            Y_DYNAMO_CHECK(rule.children.head->rule.name=="ID");
            const string ruleName = rule.children.head->lexeme.to_string();

            //------------------------------------------------------------------
            //
            // get the rule kind
            //
            //------------------------------------------------------------------
            Y_DYNAMO_CHECK(rule.children.tail);
            Y_DYNAMO_CHECK(rule.children.tail!=rule.children.head);
            const string ruleKind = rule.children.tail->rule.name;

            //------------------------------------------------------------------
            //
            // create the top-level compound
            //
            //------------------------------------------------------------------
            if(verbose) { indent() << "--> Rule <" << ruleName << ">/" << ruleKind << std::endl; }

            Syntax::Compound *global = 0;
            const bool first = (parser->top() == 0);
            const int  rcode = hsyn(ruleKind);
            switch( rcode )
            {
                case IS_AGG:
                    global = & parser->agg(ruleName);
                    break;

                case IS_ALT:
                    (void)  parser->alt(ruleName);
                    break;

                case IS_OOM:
                case IS_ZOM:
                case IS_OPT:
                case IS_ID: // not efficient...
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


