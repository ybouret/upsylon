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
            dict("ID",Common::C_IDENTIFIER);

            AGG  &dynamo  = aggregate("dynamo");
            RULE &stop    = mark(';');
            RULE &sep     = mark(':');
            RULE &rx      = plug<Lexical::jString>("rx");
            RULE &rs      = plug<Lexical::rString>("rs");
            RULE &str     = choice(rx,rs);
            RULE &zom_str = zeroOrMore(str);
            RULE &rid     = term("rid","{ID}");
            RULE &carret  = term('\x5e');
            RULE &ops_str = join(str,optional(carret));

            ALT &itm      = alternate("itm");

            //------------------------------------------------------------------
            // dynamo = module + items
            //------------------------------------------------------------------
            dynamo << join( term("module","[.]{ID}"), stop ) << zeroOrMore(itm);

            //------------------------------------------------------------------
            // Declare the Alias
            //------------------------------------------------------------------
            itm << ( aggregate("aka") << rid << sep << ops_str << stop);

            //------------------------------------------------------------------
            // Declare the Rule Interface
            //------------------------------------------------------------------
            {
                CMP &rule = (aggregate("rule") << rid << optional( term('!') ) << sep);
                {

                }
                rule << stop;
                itm << rule;
            }

            //------------------------------------------------------------------
            // Declare the Lexical Rules : plugin and lexical
            //------------------------------------------------------------------
            {
                RULE &lid = term("lid","@{ID}");
                itm << ( aggregate("plg") << lid << sep << rid     << stop);
                itm << ( aggregate("lxr") << lid << sep << zom_str << stop);
            }

            //------------------------------------------------------------------
            // Declare the Command Rules
            //------------------------------------------------------------------
            {
                RULE &cid = term("cid","%{ID}");
                itm << (aggregate("cmd") << cid << zeroOrMore(rs) << stop);
            }
            
            //------------------------------------------------------------------
            // Extraneous Lexical Only Rules
            //------------------------------------------------------------------
            hook<Lexical::CXX_Comment>(**this,"CXX_Comment");
            hook<Lexical::C_Comment>(  **this,"C_Comment");
            (**this).endl( "[:endl:]"  );
            (**this).drop( "[:blank:]" );

            //------------------------------------------------------------------
            // finalize
            //------------------------------------------------------------------
            end();
        }
    }
}
