#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {

        void DynamoGenerator:: declAlias( const DynamoNode &alias )
        {
            //__________________________________________________________________
            //
            // sanity check
            //__________________________________________________________________
            assert( "aka" == alias.name );
            assert( parser.is_valid()   );

            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[ALIAS]" << std::endl);
            if( alias.type != DynamoInternal ) throw exception("{%s} unexpected terminal alias", **(parser->name));

            //__________________________________________________________________
            //
            // get name
            //__________________________________________________________________
            const DynamoNode *node = alias.children().head;
            const string aliasName = getRID(node,"alias name");
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_name='" << aliasName << "'" << std::endl );


            //__________________________________________________________________
            //
            // get string
            //__________________________________________________________________
            const string aliasExpr = getSTR(node=node->next,"alias string");
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_expr='" << aliasExpr << "'" << std::endl );

            if((node=node->next))
            {
                // check modifier is '^'
                Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_operator" << std::endl);
                const string aliasMod = getContent(node, "^", "alias modifier");
                if(aliasMod.size()>0)       throw exception("{%s} unexpected alias '%s' modifier content='%s'", **(parser->name), *aliasName, *aliasMod);
                if(NULL!=(node->next)) throw exception("{%s} unexpected extraneous child for alias '%s'",  **(parser->name), *aliasName);
            }
            else
            {
                // nothing to do
                Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_standard" << std::endl);
            }

            //__________________________________________________________________
            //
            // declare it
            //__________________________________________________________________

            Syntax::Terminal &t = parser->term(aliasName,aliasExpr);
            if(node) t.op();
            storeDecl(t);
        }

    }
}
