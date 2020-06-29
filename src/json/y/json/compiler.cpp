
#include "y/json/compiler.hpp"
#include "y/jargon/lexical/plugin/jstring.hpp"
#include "y/jargon/lexical/plugin/error.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace JSON {
        
        using namespace Jargon;
        
        static const char *tkw[] =
        {
            "string",
            "number",
            "true",
            "false",
            "null"
        };
        
        static const char *ikw[] =
        {
            "pair",
            "empty array",
            "array",
            "empty object",
            "object"
        };
        
#define Y_JSON_NUMBER_IP "[:digit:]+"
#define Y_JSON_NUMBER_FP "([.][:digit:]+)?"
        
#define Y_JSON_NUMBER Y_JSON_NUMBER_IP Y_JSON_NUMBER_FP
        
        Compiler:: Compiler() :
        Jargon::Parser("JSON"),
        Jargon::Evaluator(title),
        value(),
        vArray(),
        vObject(),
        terminalHash( Y_MPERF_FOR(tkw) ),
        internalHash( Y_MPERF_FOR(ikw) )
        {
            //__________________________________________________________________
            //
            // top level: array|object
            //__________________________________________________________________
            Alternate &json   = alt("JSON");

            //__________________________________________________________________
            //
            // shared terminals
            //__________________________________________________________________
            Axiom     &COMA   = mark(',');
            Axiom    &STRING  = plug(Lexical::jString::Type, "string");
            {
                Alternate &VALUE = alt("value");
                {
                    VALUE << STRING;
                    VALUE << term("number", Y_JSON_NUMBER );
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
                    Axiom &LBRACE  = mark('{');
                    Axiom &RBRACE  = mark('}');
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
            
            //__________________________________________________________________
            //
            // lexical rules
            //__________________________________________________________________
            endl("endl",  "[:endl:]");
            drop("blank", "[:blank:]");

            //__________________________________________________________________
            //
            // error on any invalid starting token
            //__________________________________________________________________
            grab(Lexical::Error::Type, "JSON Error" );

            //__________________________________________________________________
            //
            // compile all this
            //__________________________________________________________________
            compile();
        }
        
        Compiler:: ~Compiler() throw()
        {
            
        }
        
        void Compiler:: onInit()
        {
            value = NullType;
            vArray.free();
            vObject.free();
        }
        
        void Compiler:: onQuit() 
        {
            if(1!=vArray.size()) throw exception("malformed JSON");
            value.swapWith(vArray.back());
            vArray.free();
            vObject.free();
        }
        
    }
    
}

#include "y/jargon/token/convert.hpp"

namespace upsylon {
    
    namespace JSON
    {
        
        void Compiler:: onTerminal(const Jargon::Tag   &tag,
                                   const Jargon::Token &token)
        {
            //JSON::Evaluator::onTerminal(tag, token);
            const string &id = *tag;
            switch( terminalHash( id ) )
            {
                case 0: assert("string" == id);
                {
                    String target(token.size,as_capacity,false);
                    Jargon::TokenConvert::To(target, token);
                    vArray.push(target);
                } break;
                    
                case 1: assert("number" == id );
                {
                    Number number = 0;
                    Jargon::TokenConvert::To(number,token);
                    vArray.push(number);
                } break;
                    
                case 2: assert("true"  == id); vArray.push(TrueType);  break;
                case 3: assert("false" == id); vArray.push(FalseType); break;
                case 4: assert("null"  == id); vArray.push(NullType);  break;
                    
                default:
                    throw exception("unknow JSON::Value '%s'", *id);
            }
        }
        
        
        void Compiler:: onOperator(const Jargon::Tag &tag,const Jargon::Token &,const size_t)
        {
            throw exception("%s: unexpected Operator<%s>", **title, **tag);
        }

        
        void Compiler:: onInternal(const Jargon::Tag &tag, const size_t n)
        {
            //JSON::Evaluator::onInternal(tag,n);
            const string &id = *tag;
            switch( internalHash(id) )
            {
                case 0: assert("pair"==id);
                {
                    if(2!=n)            throw exception("invalid #args=%u for JSON::%s", unsigned(n), *id);
                    if(vArray.size()<2) throw exception("not enough values to build a JSON::%s", *id);
                    Value v;
                    v.swapWith(vArray.back()); vArray.pop_back();
                    Value s;
                    s.swapWith(vArray.back()); vArray.pop_back();
                    if(s.type!=IsString) throw exception("got '%s' for JSON::Pair label!!", s.typeName());
                    Pair p = new _Pair(s.as<String>() );
                    p->value.swapWith(v);
                    vObject.push_back(p);
                } break;
                    
                case 1: assert("empty array"==id);
                    if(n>0) throw exception("invalid #args=%u for JSON::%s", unsigned(n),*id);
                    vArray.push(ArrayType);
                    break;
                    
                case 2: assert("array"==id); {
                    if(n<=0)            throw exception("invalid #args=%u for JSON::%s", unsigned(n),*id);
                    if(vArray.size()<n) throw exception("not enough values to build a JSON::%s", *id);
                    Value tmp( ArrayType );
                    {
                        Array &arr = tmp.as<Array>();
                        for(size_t i=n;i>0;--i)
                        {
                            arr.push(NullType);
                            vArray.pop(arr.back());
                        }
                        arr.reverse();
                    }
                    vArray.push(NullType);
                    vArray.back().swapWith(tmp);
                } break;
                    
                    
                case 3: assert("empty object"==id); {
                    if(n!=0) throw exception("invalid #args=%u for JSON::%s", unsigned(n),*id);
                    vArray.push(ObjectType);
                } break;
                    
                case 4 : assert("object"==id); {
                    if(n<=0)             throw exception("invalid #args=%u for JSON::%s", unsigned(n),*id);
                    if(vObject.size()<n) throw exception("not enough pairs to build a JSON::%s", *id);
                    Object      &obj = vArray.push(ObjectType);
                    
                    for(size_t i=n;i>0;--i)
                    {
                        const Pair &p = vObject.back();
                        if( !obj.insert(p) )
                        {
                            throw exception("multiple label '%s' in JSON::%s", *(p->label),*id);
                        }
                        vObject.pop_back();
                    }
                    
                    obj.reverse();
                    
                    
                } break;
                    
                default:
                    throw exception("unknown JSON '%s'", *id);
            }
        }
        
        
    }
    
    
}
