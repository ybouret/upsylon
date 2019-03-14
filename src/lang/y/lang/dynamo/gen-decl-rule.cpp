
#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {

        void DynamoGenerator:: declRule(  DynamoNode &rule )
        {
            //__________________________________________________________________
            //
            // sanity check
            //__________________________________________________________________
            assert( "rule" == rule.name );
            assert( parser.is_valid()   );

            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[RULE]" << std::endl);
            if( rule.type != DynamoInternal ) throw exception("{%s} unexpected terminal rule", **(parser->name));

            //__________________________________________________________________
            //
            // get name
            //__________________________________________________________________
            const DynamoNode *node     = rule.children().head;
            const string      ruleName = getRID(node,"rule name");
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_name='" << ruleName << "'" << std::endl );


        }

    }

}


