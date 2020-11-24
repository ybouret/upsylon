
#include "y/json/analyzer.hpp"
#include "y/string/convert.hpp"

namespace upsylon
{
    namespace JSON
    {
        Analyzer:: ~Analyzer() throw()
        {

        }

        static const char *Terminals[] =
        {
            "null",     // 0
            "true",     // 1
            "false",    // 2
            "number",   // 3
            "string"    // 4
        };

        static const char *Internals[] =
        {
            "pair",          // 0
            "empty_array",   // 1
            "heavy_array",   // 2
            "empty_object",  // 3
            "heavy_object"
        };

        void Analyzer:: reset() throw()
        {
            vstack.free();
            pstack.free();
        }

        Analyzer:: Analyzer(const Jive::Tag &id) :
        Jive::Syntax::Analyzer(id),
        vstack(),
        pstack(),
        vhash(Y_MPH_WORDS(Terminals)),
        ihash(Y_MPH_WORDS(Internals))
        {
            //vhash.get_root().graphViz("json-vhash.dot");
            //ihash.get_root().graphViz("json-ihash.dot");
        }

        int Analyzer:: onTerminal(const string &id, const Jive::Lexeme &data)
        {
            //(void)Jive::Syntax::Analyzer::onTerminal(id,data);
            const unsigned h = vhash(id);
            switch( h )
            {
                case 0: assert( Terminals[h] == id );
                    vstack.push( NullType );
                    break;

                case 1: assert( Terminals[h] == id );
                    vstack.push(TrueType);
                    break;

                case 2: assert( Terminals[h] == id);
                    vstack.push(FalseType);
                    break;

                case 3: assert( Terminals[h] == id);
                {
                    const string s = data.toString();
                    const double x = string_convert::to<double>(s,Terminals[h]);
                    vstack.push(x);
                } break;

                case 4: assert( Terminals[h] == id);
                {
                    const string s = data.toString();
                    vstack.push(s);
                } break;

                default:
                    throw exception("%s unhandled <%s>", **name, *id);
            }

            return 0;
        }

        int  Analyzer:: onInternal(const string &id, const size_t  size)
        {
            //(void)Jive::Syntax::Analyzer::onInternal(id,size);
            const unsigned h = ihash(id);
            switch(h)
            {
                    //----------------------------------------------------------
                    // pair
                    //----------------------------------------------------------
                case 0: assert( Internals[h] == id);
                {
                    // check call
                    if(size!=2)          throw exception("invalid <%s> #args=%u != 2", *id, unsigned(size) );

                    
                    // check stack
                    const size_t n = vstack.size();
                    if(n<2) throw exception("not enough values (%u) for <%s>",unsigned(n),*id);
                    const Value  &v = vstack[n];
                    const Value  &k = vstack[n-1];
                    if(k.type!=IsString)
                    {
                        throw exception("invalid <%s> key type '%s'", *id, k.typeName());
                    }
                    const Pair p = new _Pair(k.as<String>(),v);
                    pstack.push_back(p);
                    vstack.pop();
                    vstack.pop();
                } break;

                    //----------------------------------------------------------
                    // empty_array
                    //----------------------------------------------------------
                case 1: assert( Internals[h] == id);
                    if(size!=0) throw exception("invalid size=0 for <%s>", *id);
                    vstack.push(ArrayType);
                    break;

                    //----------------------------------------------------------
                    // heavy_array
                    //----------------------------------------------------------
                case 2: assert( Internals[h] == id );
                {
                    const size_t n = vstack.size();
                    if(size>n) throw exception("not enough data (%u) for <%s>", unsigned(n), *id);
                    Value      value = ArrayType;
                    {
                        Array     &arr   = value.as<Array>();
                        for(size_t i=size;i>0;--i)
                        {
                            arr.xpush(vstack.back());
                            vstack.pop();
                        }
                        arr.reverse();
                    }
                    vstack.xpush(value);
                } break;

                    //----------------------------------------------------------
                    // empty object
                    //----------------------------------------------------------
                case 3: assert( Internals[h] == id);
                    if(size!=0) throw exception("invalid size=0 for <%s>", *id);
                    vstack.push( ObjectType );
                    break;

                    //----------------------------------------------------------
                    // heavy object
                    //----------------------------------------------------------
                case 4: assert( Internals[h] == id);
                {
                    if(pstack.size()<size) throw exception("not enough pairs to create <%s>", *id);
                    Value value = ObjectType;
                    {
                        Object &obj = value.as<Object>();
                        for(size_t i=size;i>0;--i)
                        {
                            const Pair &p = pstack.back();
                            if(!obj.insert(p))
                            {
                                throw exception("multiple object key=\"%s\"",*(p->key()));
                            }
                            pstack.pop_back();
                        }
                        obj.reverse();
                    }
                    vstack.xpush(value);
                } break;

                default:
                    throw exception("%s unhandled call to <%s>", **name, *id);
            }


            return 0;
        }

    }
}

