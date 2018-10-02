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
            const string label = rule.children.head->lexeme.to_string();
            if(verbose) { indent() << "--> Rule <" << label << ">" << std::endl; }

        }

    }

}


