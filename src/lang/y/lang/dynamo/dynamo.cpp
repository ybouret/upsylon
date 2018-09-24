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
            // Extra Lexical Rules
            //__________________________________________________________________
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

