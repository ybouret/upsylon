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
            // setup dynamo
            //__________________________________________________________________
            AGG &dynamo = agg("dynamo");

            dict("HEAD","[_[:alpha:]]");
            dict("TAIL","[:word:]*");
            dict("ID",  "{HEAD}{TAIL}").GraphViz("id.dot");

            RULE &END = mark(';');

            //__________________________________________________________________
            //
            // all starts with a module
            //__________________________________________________________________
            dynamo << ( agg("Module") << term("ModuleID","[.]{ID}") << END);


            //__________________________________________________________________
            //
            // Extra Lexical Rules
            //__________________________________________________________________
            hook<Lexical::CXX_Comment>("Single Line Comment");
            hook<Lexical::C_Comment>(  "Multi-Lines Comment");
            root.drop("[:blank:]");
            root.endl("[:endl:]");

        }

        Dynamo:: Parser:: ~Parser() throw()
        {
        }

        

    }
}

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

