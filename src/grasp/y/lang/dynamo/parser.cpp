#include "y/lang/dynamo/parser.hpp"
#include "y/lang/lexical/plugin/strings.hpp"
#include "y/lang/lexical/plugin/comment.hpp"

namespace upsylon
{
    namespace Lang
    {
        DynamoParser:: ~DynamoParser() throw()
        {


        }

        DynamoParser:: DynamoParser() : Syntax::Parser("Dynamo")
        {
            setVerbose(true);

            topLevel( oneOrMore( term("ID","[:alnum:]+" ) ) );


            //------------------------------------------------------------------
            // Lexical Only Rules
            //------------------------------------------------------------------
            hook<Lexical::CXX_Comment>(**this,"CXX");
            (**this).endl( "[:endl:]"  );
            (**this).drop( "[:blank:]" );

            end();
        }
    }
}
