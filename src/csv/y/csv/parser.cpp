
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

            Aggregate   &file        = agg(label);
            const Axiom &TEXTDATA    = terminal("TEXTDATA", "[\\x20-\\x21\\x23-\\x2B\\x2D-\\x7E]");
            const Axiom &DQUOTE      = terminal("DQUOTE",'"');
            const Axiom &COMMA       = terminal("COMMA",',');
            const Axiom &CRLF        = end_line("CRLF","\r\n");
            const Axiom &CR          = new_line("CR","\\x0D");
            const Axiom &LF          = new_line("LF","\\x0A");
            const Axiom &non_escaped = repeat("non_escaped", TEXTDATA,0,false);
            Aggregate   &escaped     = agg("escaped");
            
            escaped << DQUOTE;
            {
                Alternate &esc = alt("esc");
                esc << TEXTDATA;
                esc << COMMA;
                esc << CR;
                esc << LF;
                escaped << zeroOrMore(esc);
            }
            escaped << DQUOTE;
            
            const Axiom &field  = ( alt("field") << non_escaped << escaped );
            const Axiom &record = ( agg("record")<< field << zeroOrMore( cat(COMMA,field) ) );
            
            file << record;
            file << zeroOrMore( cat(CRLF,record) );
            file << option(CRLF);
            
            graphViz("csv-grammar.dot");
            
            displayTerminals(std::cerr,this);
            
            validate(this);
        }
        

    }

}
