#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {

        void DynamoGenerator:: declLexical( const DynamoNode &lxr    )
        {
            //__________________________________________________________________
            //
            // sanity check
            //__________________________________________________________________
            assert( "lxr" == lxr.name );
            assert( parser.is_valid()   );

            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[LEX]" << std::endl);
            if( lxr.type != DynamoInternal ) throw exception("{%s} unexpected lexer code", **(parser->name));

            //__________________________________________________________________
            //
            // get lexical instruction
            //__________________________________________________________________
            const DynamoNode *node  = lxr.children().head;
            const string      instr = getLID(node,"lexer instruction");
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_instr='" << instr << "'" << std::endl);


            //__________________________________________________________________
            //
            // find lexical function
            //__________________________________________________________________
            DynamoLexical &lex = findLexical(instr);

            //__________________________________________________________________
            //
            // build arguments
            //__________________________________________________________________
            vector<const string,DynamoMemory> args(lxr.children().size-1,as_capacity);
            for(node=node->next;node;node=node->next)
            {
                const string arg = getSTR(node,"lexical argument");
                args.push_back_(arg);
            }
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_args =" << args << std::endl);

            lex( *parser, args );

        }

    }
}

