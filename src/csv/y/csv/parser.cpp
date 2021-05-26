
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

            Aggregate   &self        = agg(label);
            const Axiom &TEXTDATA    = terminal("TEXTDATA", "[\\x20-\\x21\\x23-\\x2B\\x2D-\\x7E]");
            const Axiom &DQUOTE      = terminal("DQUOTE",'"');
            const Axiom &COMMA       = terminal("COMMA",',');
            //const Axiom &CR          = end_line("CR","\\x0D");
            //const Axiom &LR          = end_line("LF","\\x0A");
            const Axiom &non_escaped = repeat("non_escaped", TEXTDATA, 0);
            Aggregate   &escaped     = agg("escaped");
            escaped << DQUOTE;
            
            escaped << DQUOTE;


            graphViz("csv-grammar.dot");

            validate();


        }
        

    }

}
