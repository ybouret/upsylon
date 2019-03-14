#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {

        void DynamoGenerator:: declPlugin( const DynamoNode &plg )
        {
            //__________________________________________________________________
            //
            // sanity check
            //__________________________________________________________________
            assert( "plg" == plg.name );
            assert( parser.is_valid()   );

            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[PLUGIN]" << std::endl);
            if( plg.type != DynamoInternal ) throw exception("{%s} unexpected terminal plugin", **(parser->name));

            //__________________________________________________________________
            //
            // get plugin label, its terminal name
            //__________________________________________________________________
            const DynamoNode *node     = plg.children().head;
            const string      plgLabel = getLID(node,"plugin label");
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_label='" << plgLabel << "'" << std::endl );

            //__________________________________________________________________
            //
            // get plugin class for factory
            //__________________________________________________________________
            const string      plgClass = getRID(node=node->next,"plugin class");
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_class='" << plgClass << "'" << std::endl );

            //__________________________________________________________________
            //
            // check OK
            //__________________________________________________________________
            if(NULL!=(node=node->next))
            {
                throw exception("{%s} unexpected extraneous args for plugin '%s'", **(parser->name), *plgLabel);
            }

            //__________________________________________________________________
            //
            // find plugin and register corresponding syntax terminal
            //__________________________________________________________________
            DynamoPlugin     &P = findPlugin(plgClass);
            Syntax::Terminal &t = P(plgLabel,*parser);
            storeDecl(t);

        }

    }

}

