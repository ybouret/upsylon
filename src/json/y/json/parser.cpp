#include "y/json/parser.hpp"
#include "y/jive/lexical/plugin/jstring.hpp"
#include "y/jive/common-regexp.hpp"

namespace upsylon
{
    namespace JSON
    {

        Parser:: Parser() : Jive::Parser("JSON")
        {

            //------------------------------------------------------------------
            //
            // grammar
            //
            //------------------------------------------------------------------

            //------------------------------------------------------------------
            // top level
            //------------------------------------------------------------------

            Alternate   &self = alt("JSON");
            {
                const Axiom &jString = plugin<Jive::Lexical::jString>("string");
                const Axiom &jNumber = terminal("number",Jive::RegExpFor::Double);
                Alternate   &jValue  = alt("value");
                jValue << terminal("null") << terminal("true") << terminal("false") << jString << jNumber;
                const Axiom &COMA    = division(',');
                {
                    Alternate &jArray = alt("array");
                    {
                        const Axiom &LBRACK = division('[');
                        const Axiom &RBRACK = division(']');
                        jArray << ( agg("emtpy_array") << LBRACK << RBRACK );
                        jArray << ( agg("heavy_array") << LBRACK << jValue
                                   << zeroOrMore( cat(COMA,jValue) )
                                   << RBRACK);
                    }
                    jValue << jArray;
                    self   << jArray;
                }
            }




            //------------------------------------------------------------------
            //
            // lexical
            //
            //------------------------------------------------------------------
            {
                const char __endl[] = "[:endl:]";
                endl(__endl,__endl);
            }

            {
                const char __blank[] = "[:blank:]";
                drop(__blank,__blank);
            }

            graphViz("json-grammar.dot");

        }


        Parser:: ~Parser() throw()
        {

        }

    }
}

