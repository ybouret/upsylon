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
            //setVerbose(true);
            const string ruleRX = Common::C_IDENTIFIER;

            AGG  &dynamo  = aggregate("dynamo");
            RULE &stop    = mark(';');
            RULE &sep     = mark(':');
            RULE &rx      = plug<Lexical::jString>("rx");
            RULE &rs      = plug<Lexical::rString>("rs");
            RULE &str     = choice(rx,rs);
            RULE &zom_str = zeroOrMore(str);
            RULE &rid     = term("rid",ruleRX);

            //------------------------------------------------------------------
            // Declare the Module preamble
            //------------------------------------------------------------------
            {
                const string moduleRX = "[.]" + ruleRX;
                dynamo << join( term("module",moduleRX), stop );
            }


            //------------------------------------------------------------------
            // Declare the rule declaration
            //------------------------------------------------------------------
            CMP &rule = (aggregate("rule") << rid << optional( term('!') ) << sep);
            {
                
            }
            rule << stop;


            //------------------------------------------------------------------
            // Declare the Lexical Rules : plugin and lexical
            //------------------------------------------------------------------
            AGG &plg = aggregate("plg");
            AGG &lxr = aggregate("lxr");
            {
                const string lexicalRX = "@" + ruleRX;
                RULE &lid = term("lid",lexicalRX);
                plg << lid << sep << rid     << stop;
                lxr << lid << sep << zom_str << stop;
            }

            //------------------------------------------------------------------
            // Declare the Command Rules
            //------------------------------------------------------------------
            AGG &cmd = aggregate("cmd");
            {
                const string commandRX = "%" + ruleRX;
                RULE &cid = term("cid",commandRX);
                cmd << cid << zeroOrMore(rs)  << stop;
            }

            dynamo << zeroOrMore( alternate("item") << rule << plg << lxr << cmd );

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
