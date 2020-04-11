
#include "y/json/parser.hpp"
#include "y/jargon/lexical/plugin/jstring.hpp"
#include "y/jargon/lexical/plugin/error.hpp"


namespace upsylon {
    
    namespace JSON {
        
        using namespace Jargon;
        
        Parser:: Parser() : Jargon::Parser("JSON"), value()
        {
            
            // top level: array|object
            Alternate &json   = alt();
            
            // shared terminals
            Axiom     &COMA   = mark(',');
            Axiom    &STRING  = plug(type2type<Lexical::jString>(), "string");
            {
                Alternate &VALUE = alt();
                {
                    VALUE << STRING;
                    VALUE << term("number", "[:digit:]+");
                    VALUE << term("true") << term("false") << term("null");
                }
                
                {
                    Axiom &LBRACK = mark('[');
                    Axiom &RBRACK = mark(']');
                    Axiom &ZARRAY = ( agg("empty array") << LBRACK << RBRACK );
                    Axiom &ARRAY  = ( agg("array")
                                     << LBRACK
                                     << VALUE
                                     << zom( cat(COMA,VALUE) )
                                     << RBRACK
                                     );
                    Axiom &ARRAYS = choice(ZARRAY,ARRAY);
                    VALUE << ARRAYS;
                    json  << ARRAYS;
                }
                
                {
                    Axiom &LBRACE = mark('{');
                    Axiom &RBRACE = mark('}');
                    Axiom &ZOBJECT = ( agg("empty object") << LBRACE << RBRACE );
                    Axiom &PAIR    = ( agg("pair")    << STRING << mark(':') << VALUE );
                    Axiom &OBJECT  = ( agg("object")  << LBRACE
                                      << PAIR
                                      << zom( cat(COMA,PAIR) )
                                      << RBRACE );
                    Axiom &OBJECTS = choice(ZOBJECT,OBJECT);
                    VALUE << OBJECTS;
                    json << OBJECTS;

                }
            }
            
            
            // lexical rules
            endl("endl",  "[:endl:]");
            drop("blank", "[:blank:]");
            
            // error on any invalid starting token
            load( type2type<Lexical::Error>(), "JSON Error" ).hook(*this);
            
            compile();
            
        }
        
        Parser:: ~Parser() throw()
        {
            
        }
        
    }
    
    
}
