

#include "y/chem/lua/io.hpp"
#include "y/ptr/auto.hpp"
#include "y/lua++/function.hpp"

namespace upsylon
{
    namespace Chemical
    {

        namespace
        {
            class LuaEq : public Equilibrium
            {
            public:
                mutable Lua::Function<double> K;
                explicit LuaEq( const string &id,
                               const string  &fn,
                               const Lua::VM &vm) :
                Equilibrium(id),
                K(fn,vm)
                {

                }

                virtual ~LuaEq() throw()
                {
                }

                inline virtual double  operator()( double t ) const
                {
                    return K(t);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(LuaEq);
            };
        }

        static inline
        void __load_equilbrium( Equilibria &eqs, Lua::VM &vm, const char *id, const Library &lib, const int count )
        {
            lua_State *L = **vm;
            assert(lua_istable(L,-1));

            //------------------------------------------------------------------
            //
            // create an equilibrium
            //
            //------------------------------------------------------------------
            auto_ptr<Equilibrium> pEq = 0;

            {
                //--------------------------------------------------------------
                //
                // get the name
                //
                //--------------------------------------------------------------
                if( LUA_TSTRING != lua_rawgeti(L,-1,1) )
                {
                    throw exception("%s#%d first item is not a string",id,count);
                }
                const string eq_name = vm->to<string>(-1);
                lua_pop(L,1);
                std::cerr << "new equilibrium '" << eq_name << "'" << std::endl;

                //--------------------------------------------------------------
                //
                // get the name
                //
                //--------------------------------------------------------------
                switch( lua_rawgeti(L,-1,2) )
                {
                    case LUA_TSTRING: {
                        const string fn = vm->to<string>(-1);
                        pEq = new LuaEq(eq_name,fn,vm);
                    } break;

                    case LUA_TNUMBER:
                        pEq = new ConstEquilibrium( eq_name, vm->to<double>(-1) );
                        break;

                    default:
                        exception("%s#%d second item it not a string or a number",id,count);
                }
                lua_pop(L,1);
            }
            assert(pEq.is_valid());

            //------------------------------------------------------------------
            //
            // get the components
            //
            //------------------------------------------------------------------
            assert(lua_istable(L,-1));
            const size_t n = lua_rawlen(L,-1);
            const char *eq = *(pEq->name);
            for(size_t i=3;i<=n;++i)
            {
                const unsigned j = unsigned(i)-2;
                if( LUA_TTABLE != lua_rawgeti(L,-1,i) )
                {
                    throw exception("%s#%u is not a table",eq,j);
                }
                // now we have the component on the stack
                if( LUA_TSTRING != lua_rawgeti(L,-1, 1) )
                {
                    throw exception("%s#%u first item is not a string",eq,j);
                }
                const string component_name = vm->to<string>(-1);
                lua_pop(L,1);

                if( LUA_TNUMBER != lua_rawgeti(L,-1,2) )
                {
                    throw exception("%s#%u second item is not a number",eq,j);
                }
                const int component_coef = vm->to<int>(-1);
                lua_pop(L,1);

                pEq->add(lib[component_name],component_coef);
                // remove the component
                lua_pop(L,1);
            }

            //------------------------------------------------------------------
            //
            // enroll the equilibrium
            //
            //------------------------------------------------------------------
            (void)eqs.enroll( pEq.yield() );


        }

        void __luaIO:: add( Equilibria &eqs, Lua::State::Pointer &vm, const string &name, const Library &lib )
        {
            lua_State *L = **vm;

            lua_settop(L,0);
            lua_getglobal(L,*name);
            if(!lua_istable(L,-1))
            {
                throw exception("no equilibria table '%s'", *name);
            }

            //__________________________________________________________________
            //
            // loop on table items = eq
            //__________________________________________________________________
            // stack now contains: -1 => table
            lua_pushnil(L);
            // stack now contains: -1 => nil; -2 => table
            int count = 0;
            while( lua_next(L,-2) )
            {
                // stack now contains: -1 => value; -2 => key; -3 => table
                ++count;
                __load_equilbrium(eqs,vm, *name, lib, count);
                // remove value
                lua_pop(L,1);
            }
            // nil
            lua_pop(L,1);
        }

    }

}


