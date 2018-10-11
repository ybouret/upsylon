#include "y/json/compiler.hpp"
#include "y/string/convert.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace
    {
#include "json.inc"
    }

    namespace JSON
    {
#include "json.def"

        void  Compiler:: load( Value &v, Lang::Module *module, const bool verbose_flag )
        {
            //__________________________________________________________________
            //
            // initialize
            //__________________________________________________________________
            pstack.free();
            vstack.free();
            verbose=verbose_flag;

            //__________________________________________________________________
            //
            // parse and walk
            //__________________________________________________________________

            compile(module);

            //__________________________________________________________________
            //
            // normally, one value on vstack
            //__________________________________________________________________
            if(vstack.size()!=1 || pstack.size()>0)
            {
                throw exception("{%s} invalid compilation (#vstack=%u,#pstack=%u)", **(parser->name), unsigned( vstack.size() ), unsigned( pstack.size() ));
            }

            v.swap_with(vstack.front());
            vstack.free();
        }



        void Compiler::onTerminal( const Lang::Lexeme &data )
        {
            if(verbose) { Analyzer::onTerminal(data); }
            const int code = terminalHash(*data.label);
            switch(code)
            {
                case JSON_null:   vstack.push(NullType); break;
                case JSON_true:   vstack.push(TrueType); break;
                case JSON_false:  vstack.push(TrueType); break;
                case JSON_string: {
                    const string s = data.to_string(1,1);
                    vstack.push(s);
                } break;
                case JSON_number:
                {
                    const string s = data.to_string();
                    const Number x = string_convert::to<double>(s,"JSON::Number");
                    vstack.push(x);
                } break;
                default:
                    break;
            }
        }

        void Compiler::onInternal(const string &name, const size_t size)
        {
            if(verbose) { Analyzer::onInternal(name,size); }
            const int code = internalHash(name);
            switch(code)
            {
                case JSON_pair: {
                    //__________________________________________________________
                    //
                    // create a pair from string/value on the stack
                    //__________________________________________________________
                    if(size!=2)         throw exception("{%s}.%s invalid size!",**(parser->name),*name);
                    if(vstack.size()<2) throw exception("{%s}.%s missing stack value",**(parser->name),*name);

                    Value v; vstack.pop(v);
                    Value l; vstack.pop(l);
                    if(l.type!=IsString) throw exception("{%s}.%s invalid label type",**(parser->name),*name);
                    {
                        const string s = l.as<String>();
                        Pair p = new _Pair(s);
                        p->value.swap_with(v);
                        pstack.push_back(p);
                    }
                } break;

                case JSON_heavy_object:
                {
                    //__________________________________________________________
                    //
                    // collect pairs to form object
                    //__________________________________________________________
                    if(size<=0)            throw exception("{%s}.%s invalid size!",**(parser->name),*name);
                    if(pstack.size()<size) throw exception("{%s}.%s missing pairs",**(parser->name),*name);
                    JSON::Object &obj = vstack.push(ObjectType);
                    for(size_t i=0;i<size;++i)
                    {
                        const Pair p = pstack.back();
                        if(!obj.insert(p))
                        {
                            throw exception("{%s}.%s mutliple label '%s",**(parser->name),*name,*(p->label));
                        }
                        pstack.pop_back();
                    }
                    obj.reverse();
                } break;

                case JSON_empty_object:
                {
                    //__________________________________________________________
                    //
                    // push a new empty object
                    //__________________________________________________________
                    if(size!=0) throw exception("{%s}.%s invalid size!",**(parser->name),*name);
                    (void)vstack.push(ObjectType);
                } break;

                case JSON_empty_array:
                {
                    //__________________________________________________________
                    //
                    // push a new empty array
                    //__________________________________________________________
                    if(size!=0) throw exception("{%s}.%s invalid size!",**(parser->name),*name);
                    (void)vstack.push(ArrayType);
                } break;

                case JSON_heavy_array:
                {
                    //__________________________________________________________
                    //
                    // collect values from the stack
                    //__________________________________________________________
                    if(size<=0)            throw exception("{%s}.%s invalid size!",**(parser->name),*name);
                    if(vstack.size()<size) throw exception("{%s}.%s missing values",**(parser->name),*name);
                    Value v    = ArrayType;
                    Array &arr = v.as<Array>();
                    arr.nil(size);
                    for(size_t i=size;i>0;--i) vstack.pop( arr[i] );
                    vstack._push(v);
                } break;

                default:
                    throw exception("{%s}.%s is unexpected",**(parser->name),*name);
            }

        }



        Compiler:: Compiler(const bool verbose) :
        Lang::DynamoX( Y_DYNAMOX(JSON), verbose ),
        vstack(),
        pstack(),
        verbose(false)
        {
        }

        Compiler:: ~Compiler() throw()
        {
        }


        
    }

}
