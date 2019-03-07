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

            AGG  &dynamo = aggregate("dynamo");
            RULE &stop   = mark(';');
            RULE &sep    = mark(':');
            RULE &rx     = plug<Lexical::jString>("rx");
            RULE &rs     = plug<Lexical::rString>("rs");
            RULE &str    = choice(rx,rs);
            RULE &rid   = term("rid",ruleRX);

            //------------------------------------------------------------------
            // Declare the module preamble
            //------------------------------------------------------------------
            {
                const string moduleRX = "[.]" + ruleRX;
                dynamo << join( term("module",moduleRX), stop );
            }


            //------------------------------------------------------------------
            // Declare the Lexical rules
            //------------------------------------------------------------------
            {
                const string lexicalRX = "@" + ruleRX;
                RULE &lid = term("lid",lexicalRX);
                dynamo << zeroOrMore( aggregate("plg") << lid << sep << rid << stop );
                dynamo << zeroOrMore( aggregate("lxr") << lid << sep << zeroOrMore(str) << stop);
            }

            //------------------------------------------------------------------
            // Extraneous Lexical Only Rules
            //------------------------------------------------------------------
            hook<Lexical::CXX_Comment>(**this,"CXX_Comment");
            hook<Lexical::C_Comment>(  **this,"C_Comment");
            (**this).endl( "[:endl:]"  );
            (**this).drop( "[:blank:]" );

            end();
        }
    }
}
