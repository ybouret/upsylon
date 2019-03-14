
#include "y/lang/dynamo/generator.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace Lang
    {

        void DynamoGenerator:: declCommand( const DynamoNode &cmd )
        {
            //__________________________________________________________________
            //
            // sanity check
            //__________________________________________________________________
            assert( "cmd" == cmd.name );
            assert( parser.is_valid()   );

            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "[COMMAND]" << std::endl);
            if( cmd.type != DynamoInternal ) throw exception("{%s} unexpected terminal command", **(parser->name));

            //__________________________________________________________________
            //
            // get cmd name
            //__________________________________________________________________
            const DynamoNode *node  = cmd.children().head;
            const string      instr = getCID(node,"command name");
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_name='" << instr << "'" << std::endl);

            //__________________________________________________________________
            //
            // build arguments
            //__________________________________________________________________
            vector<const string,DynamoMemory> args(cmd.children().size-1,as_capacity);
            for(node=node->next;node;node=node->next)
            {
                const string arg = getSTR(node,"command argument");
                args.push_back_(arg);
            }
            Y_LANG_SYNTAX_VERBOSE(DynamoNode::Indent(std::cerr << "@gen",level) << "|_args =" << args << std::endl);
        }

    }

}

