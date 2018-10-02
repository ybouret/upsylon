#include "y/lang/dynamo/generator.hpp"

namespace upsylon
{
    namespace Lang
    {
        void DynamoGenerator:: onRule( const Node &rule )
        {
            static const char fn[] = "Rule";



            Y_DYNAMO_CHECK(rule.internal);
            Y_DYNAMO_CHECK(rule.rule.name=="RULE");
            Y_DYNAMO_CHECK(rule.children.size==2);

            Y_DYNAMO_CHECK(rule.children.head);
            Y_DYNAMO_CHECK(rule.children.head->terminal);
            Y_DYNAMO_CHECK(rule.children.head->rule.name=="ID");
            const string ruleName = rule.children.head->lexeme.to_string();

            Y_DYNAMO_CHECK(rule.children.tail);
            const string ruleKind = rule.children.tail->rule.name;

            if(verbose) { indent() << "--> Rule <" << ruleName << ">/" << ruleKind << std::endl; }

            Syntax::Compound *global = 0;
            
            switch( hsyn(ruleKind) )
            {
                case IS_AGG:
                    global = & parser->agg(ruleName);
                    break;

                case IS_ALT:
                    (void) parser->alt(ruleName);
                    break;

                case IS_OOM:
                case IS_ZOM:
                case IS_OPT:
                case IS_ID: // not efficient...
                    (void) parser->acting(ruleName);
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


