#include "y/json/parser.hpp"
#include "y/jive/lexical/plugin/jstring.hpp"
#include "y/jive/common-regexp.hpp"

namespace upsylon
{
    namespace JSON
    {

        Parser:: Parser() : Jive::Language::Parser("JSON")
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
                const Axiom &COMMA    = division(',');

                //--------------------------------------------------------------
                // arrays
                //--------------------------------------------------------------
                {
                    Alternate &jArray = alt("array");
                    {
                        const Axiom &LBRACK = division('[');
                        const Axiom &RBRACK = division(']');
                        jArray << ( agg("empty_array") << LBRACK << RBRACK );
                        jArray << ( agg("heavy_array") << LBRACK << jValue
                                   << zeroOrMore( cat( Axioms(COMMA,jValue) ) )
                                   << RBRACK);
                    }
                    jValue << jArray;
                    self   << jArray;
                }

                //--------------------------------------------------------------
                // objects
                //--------------------------------------------------------------
                {
                    Alternate   &jObject = alt("object");
                    const Axiom &LBRACE  = division('{');
                    const Axiom &RBRACE  = division('}');
                    jObject << (agg("empty_object")  << LBRACE << RBRACE );
                    {
                        const Axiom &jPair = (agg("pair") << jString << division(':') << jValue);
                        jObject << (
                                    agg("heavy_object") << LBRACE << jPair
                                    << zeroOrMore( cat( Axioms(COMMA,jPair) ) )
                                    << RBRACE
                                    );
                    }
                    jValue << jObject;
                    self   << jObject;
                }
            }




            //------------------------------------------------------------------
            //
            // lexical
            //
            //------------------------------------------------------------------
            drop("[:blank:]");
            endl("[:endl:]",silent);



            //graphViz("json-grammar.dot");
            validate();
        }


        Parser:: ~Parser() throw()
        {

        }

    }
}

