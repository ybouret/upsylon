
#include "y/aqua/lua/interface.hpp"
#include "y/ptr/auto.hpp"
#include "y/jive/lexical/scanner.hpp"
#include "y/jive/lexemes.hpp"

namespace upsylon
{
    namespace Aqua
    {
        using namespace Jive;

        class ComponentScanner : public Lexical::Scanner
        {
        public:
            inline explicit ComponentScanner() : Lexical::Scanner("Aqua.Component.Scanner")
            {
                emit("sgn",  "[-+]");
                emit("int",  "[:digit:]+");
                emit("chr",  "[:alpha:]([[:word:]()\\[\\]])*");
                drop("blank","[:blank:]");
            }

            inline virtual ~ComponentScanner() throw()
            {
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(ComponentScanner);
        };

        static const char fn[] = "__Lua.Load.Equilibria";

        static inline
        void __add(Equilibria    &eqs,
                   const char    *eqsName,
                   lua_State     *L,
                   const int      indx,
                   const Library &lib)
        {
            assert( LUA_TTABLE == lua_type(L,-1) );

            const int n = lua_rawlen(L,-1);
            if(n<3) throw exception("%s %s[%d] has not enough items (found %d)", fn, eqsName, indx, n);


            //------------------------------------------------------------------
            //
            // get equilibrium name
            //
            //------------------------------------------------------------------
            string name;
            {
                lua_rawgeti(L,-1,1);
                if(!lua_isstring(L,-1)) throw exception("%s %s[%d][1] is no <string> but <%s>",fn,eqsName,indx,luaL_typename(L,-1));
                name = lua_tostring(L,-1);
                lua_pop(L,1);
            }
            std::cerr << "name=" << name << std::endl;
            const char *eqName = *name;

            //------------------------------------------------------------------
            //
            // get equilibrium constant/type
            //
            //------------------------------------------------------------------
            auto_ptr<Equilibrium> pEq = 0;
            {
                lua_rawgeti(L,-1,2);
                switch(lua_type(L,-1))
                {
                    case LUA_TNUMBER:
                        pEq = new ConstEquilibrium(name,lua_tonumber(L,-1));
                        break;

                    default:
                        throw exception("%s %s.%s constant can't be <%s>",fn,eqsName,eqName,luaL_typename(L,-1));
                }
                lua_pop(L,1);
            }

            ComponentScanner scan;

            for(int i=3,j=1;i<=n;++i,++j)
            {
                lua_rawgeti(L,-1,i);
                const string cname = vformat("component[%d]",j);
                if(!lua_isstring(L,-1)) exception("%s %s.%s %s is not <string> but <%s>",fn,eqsName,eqName,*cname,luaL_typename(L,-1));
                const string component = lua_tostring(L,-1);
                lua_pop(L,1);
                std::cerr << "component=" << component << std::endl;

                Lexemes lexemes;
                {
                    Source             source(Module::OpenData(cname,component));
                    Lexical::Directive directive=0;
                    {
                        Lexeme *lx = 0;
                        while( NULL != (lx=scan.probe(source,directive)))
                        {
                            lexemes.push_back( lx );
                        }
                    }

                    if(lexemes.size<=0) throw exception("%s %s.%s empty %s",fn,eqsName,eqName,*cname);

                    // check sign
                    bool    neg = false;
                    Lexeme *lex = lexemes.head;
                    if(*(lex->label)=="sgn")
                    {
                        assert(1==lex->size);
                        //std::cerr << "sgn=" << *lex << std::endl;
                        switch(lexemes.head->head->code)
                        {
                            case '-': neg = true; break;
                            default:
                                break;
                        }
                        lex=lex->next;
                    }

                    // check factor
                    if(!lex)
                    {
                        throw exception("%s %s.%s empty %s after sign",fn,eqsName,eqName,*cname);
                    }
                    Int fac = 1;
                    if( *(lex->label)== "int")
                    {
                        //std::cerr << "int=" << *lex << std::endl;
                        fac = 0;
                        for(const Char *ch=lex->head;ch;ch=ch->next)
                        {
                            fac *= 10;
                            fac += char(ch->code) - '0';
                        }
                        lex=lex->next;
                    }
                    if(!lex)
                    {
                        throw exception("%s %s.%s empty %s after factor",fn,eqsName,eqName,*cname);
                    }
                    if(neg) fac=-fac;
                    //std::cerr << "fac=" << fac << std::endl;

                    // build id
                    string id;
                    while(lex)
                    {
                        for(const Char *ch=lex->head;ch;ch=ch->next)
                        {
                            id += char(ch->code);
                        }
                        lex=lex->next;
                    }
                    //std::cerr << "id=" << id << std::endl;

                    (*pEq)( lib[id], fac);

                }
                


            }

            //------------------------------------------------------------------
            //
            // insert into database
            //
            //------------------------------------------------------------------
            (void) eqs( pEq.yield() );


        }

        void __Lua:: Load(Equilibria    &eqs,
                          const string  &name,
                          const Library &lib,
                          Lua::VM       &vm )
        {

            //------------------------------------------------------------------
            //
            // prepare lua
            //
            //------------------------------------------------------------------
            lua_State *L = **vm;
            lua_settop(L,0);
            const char *eqsName = *name;
            // get lib as table
            lua_getglobal(L,eqsName);
            if( !lua_istable(L,-1) )
            {
                throw exception("%s %s is <%s> instead of <table>",fn,eqsName,luaL_typename(L,-1));
            }

            //------------------------------------------------------------------
            //
            // parse all items
            //
            //------------------------------------------------------------------
            const int n = lua_rawlen(L,-1);
            for(int indx=1;indx<=n;++indx)
            {
                const int t = lua_rawgeti(L,-1,indx);
                if( LUA_TTABLE != t)
                {
                    throw exception("%s %s[%d] is not <table> but <%s>",fn,eqsName,indx,lua_typename(L,t));
                }
                __add(eqs,eqsName,L,indx,lib);
                lua_pop(L,1);
            }

        }

        void __Lua:: Load(Equilibria    &eqs,
                          const char    *name,
                          const Library &lib,
                          Lua::VM       &vm)
        {
            const string _(name);
            Load(eqs,_,lib,vm);
        }

    }
}
