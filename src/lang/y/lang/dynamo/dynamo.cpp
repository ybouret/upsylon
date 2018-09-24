#include "y/lang/dynamo/dynamo.hpp"
#include "y/lang/lexical/plugin/comment.hpp"
#include "y/lang/lexical/plugin/strings.hpp"

namespace upsylon
{
    namespace Lang
    {

        Dynamo:: Parser:: Parser() : Syntax::Parser("Dynamo")
        {

            //__________________________________________________________________
            //
            //
            // setup dynamo
            //
            //__________________________________________________________________
            AGG &dynamo = agg("dynamo");

            dict("HEAD","[_[:alpha:]]");
            dict("TAIL","[:word:]*");
            dict("ID",  "{HEAD}{TAIL}").GraphViz("id.dot");

            RULE &END      = mark(';');
            RULE &COLON    = mark(':');
            RULE &CSTRING  = plug<Lexical::jString>("cstring");
            RULE &RSTRING  = plug<Lexical::rString>("rstring");
            // this is a trick: the rewrite will erase op_alias
            // if the operator is detected...
            RULE &OPERATOR = op('^');
            RULE &OSTRING = (design("ostring") << RSTRING << optional(OPERATOR));

            //__________________________________________________________________
            //
            //
            // all starts with a module
            //
            //__________________________________________________________________
            dynamo << ( acting("Module") << term("ModuleID","[.]{ID}") << END);

            //__________________________________________________________________
            //
            //
            // Then some Grammar rules
            //
            //__________________________________________________________________
            AGG &G = agg("G");
            {
                RULE &G_ID  = term("G_ID","{ID}");

                G <<  G_ID << COLON;

                G << OSTRING;

                G << END;

            }

            //__________________________________________________________________
            //
            //
            // possible lexical instruction
            //
            //__________________________________________________________________
            AGG &L = agg("L");
            {
                L << term("L_ID","@{ID}") << COLON;
                L << oneOrMore( choice(CSTRING,RSTRING) );
                L << END;
            }

            dynamo << zeroOrMore( choice(G,L) );

            //__________________________________________________________________
            //
            //
            // Extra Lexical Rules
            //
            //__________________________________________________________________
            hook<Lexical::CXX_Comment>("Single Line Comment");
            hook<Lexical::C_Comment>(  "Multi-Lines Comment");
            root.drop("[:blank:]");
            root.endl("[:endl:]");

            checkValidity();
            dict.release();
        }

        Dynamo:: Parser:: ~Parser() throw()
        {
        }

        

    }
}

#if 0
namespace upsylon
{
    namespace Lang
    {

        Dynamo:: Generator:: Generator() : Lang::Compiler( new Dynamo::Parser() )
        {

        }

        Dynamo:: Generator:: ~Generator() throw()
        {
        }
        


    }

}
#endif
