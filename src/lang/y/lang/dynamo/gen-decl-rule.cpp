
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
            const DynamoList &self     = rule.children();
            const DynamoNode *node     = self.head;
            const string      ruleName = getRID(node,"rule name");
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_name='" << ruleName << "'" << std::endl );


            if(self.size==2&&node->next->name=="alt")
            {
                storeDecl( parser->alternate(ruleName) );
            }
            else
            {
                storeDecl( parser->aggregate(ruleName) );
            }
        }

    }

}


