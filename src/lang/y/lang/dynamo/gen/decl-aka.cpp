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
            // get class
            //__________________________________________________________________
            node=node->next;
            if(!node) throw exception("{%s} missing alias '%s' description", **(parser->name), *aliasName);
            const string &aliasType = node->name;
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_type='" << aliasType << "'" << std::endl );

            
            bool isOperator = false;
            switch( operH(aliasType) )
            {
                case 0: assert("op"==aliasType);
                    // set to operator and change node
                    isOperator=true;
                    if(node->type!=DynamoInternal) throw exception("{%s} alias '%s' is not internal",**(parser->name),*aliasName);
                    node=node->children().head;
                    if(NULL!=(node->next)) throw exception("{%s} alias operator '%s' has extraneous data!",**(parser->name),*aliasName);

                    // checking structure
                    //if(!node->next)              throw exception("{%s} alias operator '%s' is missing specification!",**(parser->name),*aliasName);
                    //if(node->next->name != "^" ) throw exception("{%s} alias operator '%s' has invalid spec '%s'",**(parser->name),*aliasName,*(node->next->name));
                    //if(NULL!=(node->next->next)) throw exception("{%s} alias operator '%s' has extraneous data!",**(parser->name),*aliasName);
                    break;

                case 1: assert("rx"==aliasType);
                    break;

                case 2: assert("rs"==aliasType);
                    break;

                default:
                    throw exception("{%s} corrupted alias '%s' type=<%s>", **(parser->name), *aliasName, *aliasType);
            }


            const string aliasExpr = getSTR(node,"alias string");
            Syntax::Terminal &t = parser->term(aliasName,aliasExpr);
            if(isOperator) t.op();
            storeDecl( t );


        }

    }
}
