#include "y/lang/dynamo/parser.hpp"
#include "y/lang/lexical/plugin/strings.hpp"
#include "y/lang/lexical/plugin/comment.hpp"

#include "y/lang/pattern/common.hpp"

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
            const string ruleRX = Common::C_IDENTIFIER;

            AGG  &DYNAMO = aggregate("dynamo");
            RULE &END    = mark(';');

            //------------------------------------------------------------------
            // declare the module
            //------------------------------------------------------------------
            {
                const string moduleRX = "[.]" + ruleRX;
                DYNAMO << join( term("module",moduleRX), END );
            }


            //------------------------------------------------------------------
            // Lexical Only Rules
            //------------------------------------------------------------------
            hook<Lexical::CXX_Comment>(**this,"CXX_Comment");
            hook<Lexical::C_Comment>(  **this,"C_Comment");
            (**this).endl( "[:endl:]"  );
            (**this).drop( "[:blank:]" );

            end();
        }
    }
}
