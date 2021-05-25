
#include "y/csv/parser.hpp"
#include "y/jive/lexical/plugin/jstring.hpp"
#include "y/jive/common-regexp.hpp"

namespace upsylon
{

    namespace CSV
    {

        Parser:: ~Parser() throw()
        {
        }

        Parser:: Parser() : Jive::Parser("CSV")
        {

            Aggregate &self = agg(label);
            {


                const Axiom &Field = terminal("Field","[:alpha:]+");


                {
                    Aggregate   &Line  = agg("Line");
                    Line << Field << zeroOrMore( cat( division(','), Field ));
                    self << zeroOrMore(Line);
                }

                // lexical
                { const char __endl[]  = "[:endl:]";  endl(__endl,__endl);   }
                { const char __blank[] = "[:blank:]"; drop(__blank,__blank); }
            }

            graphViz("csv-grammar.dot");

            validate();


        }
        

    }

}
