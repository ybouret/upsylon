
#include "y/chem/lua/io.hpp"

namespace upsylon
{
    namespace Chemical
    {

        static inline
        void __load_species( Library &lib, Lua::State::Pointer &vm, const char *id, const int count )
        {
            lua_State *L = **vm;

            std::cerr << "load " << id << "#" << count << std::endl;
            if(!lua_istable(L,-1))
            {
                throw exception("%s#%d is not a table", id, count);
            }
            if( LUA_TSTRING != lua_rawgeti(L,-1,1) )
            {
                throw exception("%s#%d first item is not a string",id,count);
            }
            const string sp_name = vm->to<string>(-1);
            lua_pop(L,1);
            if( LUA_TNUMBER != lua_rawgeti(L,-1,2) )
            {
                throw exception("%s#%d second item is not a number",id,count);
            }
            const int sp_z = vm->to<int>(-1);
            lua_pop(L,1);
            (void)lib(sp_name,sp_z);
        }

        void __luaIO:: add(Library             &lib,
                           Lua::State::Pointer &vm,
                           const string        &name )
        {
            static const char fn[] = "__luaIO::add: ";
            lua_State  *L  = **vm;  assert(L);
            const char *id = *name; assert(id);
            lua_settop(L,0);
            lua_getglobal(L,id);
            if( !lua_istable(L,-1) )
            {
                throw exception("%s%s is not a table",fn,id);
            }

            //__________________________________________________________________
            //
            // loop on table items
            //__________________________________________________________________
            // stack now contains: -1 => table
            lua_pushnil(L);
            // stack now contains: -1 => nil; -2 => table
            int count = 0;
            while( lua_next(L,-2) )
            {
                // stack now contains: -1 => value; -2 => key; -3 => table
                ++count;
                __load_species(lib,vm,id,count);
                // remove value
                lua_pop(L,1);
            }
            // nil
            lua_pop(L,1);

        }


    }
}


