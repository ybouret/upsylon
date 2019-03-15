
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
            DynamoList  &self     = rule.children();
            const string ruleName = getRID(self.head,"rule name");
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_name='" << ruleName << "'" << std::endl );

            //__________________________________________________________________
            //
            // check second node
            //__________________________________________________________________
            bool isDesignNode=false;
            if(self.size>1)
            {
                DynamoNode *second = self.head->next;
                if(second->name == "!")
                {
                    if(second->type!=DynamoTerminal) throw exception("{%s} corrupted terminal '!'", **(parser->name));
                    isDesignNode = true;
                    delete self.unlink(second);
                }
            }

            if(self.size==2&&self.head->next->name=="alt")
            {
                storeDecl( parser->alternate(ruleName) );
            }
            else
            {
                if(isDesignNode)
                {
                    storeDecl( parser->design(ruleName) );
                }
                else
                {
                    storeDecl( parser->aggregate(ruleName) );
                }
            }
        }

    }

}


