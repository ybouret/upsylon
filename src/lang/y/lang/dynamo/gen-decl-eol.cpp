#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {
        void DynamoGenerator:: declEOL(const DynamoNode &eol)
        {
            //__________________________________________________________________
            //
            // sanity check
            //__________________________________________________________________
            assert( "eol" == eol.name );
            assert( parser.is_valid()   );

            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[EOL]" << std::endl);
            if( eol.type != DynamoInternal ) throw exception("{%s} unexpected terminal EOL", **(parser->name));

            //__________________________________________________________________
            //
            // get name
            //__________________________________________________________________
            const DynamoNode *node    = eol.children().head;
            const string      eolName = getEID(node,"EOL name");
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_name='" << eolName << "'" << std::endl );


            //__________________________________________________________________
            //
            // get string
            //__________________________________________________________________
            const string eolExpr = getSTR(node=node->next,"EOL string");
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_expr='" << eolExpr << "'" << std::endl );

            storeDecl(parser->endl(eolName,eolExpr));
        }


    }
}

