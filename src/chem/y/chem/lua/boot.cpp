
#include "y/chem/lua/io.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Chemical
    {


        static inline
        void __load_constraint(Boot::Loader  &loader,
                               Lua::VM        &vm,
                               const char     *id,
                               Library        &lib,
                               const int      count)
        {
            lua_State *L = **vm;
            assert(count>0);
            assert(lua_istable(L,-1) );


            switch( lua_rawgeti(L,-1,1) )
            {
                case LUA_TSTRING:
                {
                    const string label = vm->to<string>(-1); // get the label
                    lua_pop(L,1);                            // remove the label

                    std::cerr << "new named constraint " << label << std::endl;
                    if( "E/N" == label )
                    {
                        loader.electroneutrality(lib);
                        return;
                    }

                    if( "OSM" == label )
                    {
                        // get osm value
                        if( LUA_TNUMBER != lua_rawgeti(L,-1,2) )
                        {
                            std::cerr << "not a number..." << std::endl;
                            throw exception("%s#%d: OSM needs a number", id, count);

                        }
                        const double osm = vm->to<double>(-1);
                        lua_pop(L,1); // remove osm value
                        loader.osmolarity(osm,lib);
                        return;
                    }

                    throw exception("%s#%d invalid named constraint '%s'",id,count,*label);
                } return;

                case LUA_TNUMBER:
                    break; // normal constraint

                default:
                    throw exception("%s#%d: constraint first item is not a number or a string",id,count);
            }

            // create a constraint from the extracted value
            assert(lua_isnumber(L,-1));
            Boot::Constraint &cc = loader.create( vm->to<double>(-1) );
            lua_pop(L,1);

            // loop on desriptors
            assert(lua_istable(L,-1));
            const unsigned n = lua_rawlen(L,-1);
            for(unsigned i=2,j=1;i<=n;++i,++j)
            {
                // get the descriptor
                if( LUA_TTABLE != lua_rawgeti(L,-1,i) )
                {
                    throw exception("%s#%d constraint item#%u is not a table",id,count,j);
                }

                // get the species names
                if( LUA_TSTRING != lua_rawgeti(L,-1,1) )
                {
                    throw exception("%s#%d constraint item#%u first item is not a string",id,count,j);
                }
                const string sp_name = vm->to<string>(-1);
                lua_pop(L,1); // remove the species name

                // get the species weight
                if( LUA_TNUMBER != lua_rawgeti(L,-1,2) )
                {
                    throw exception("%s#%d constraint item#%u second item is not a string",id,count,j);
                }
                const int sp_weight = vm->to<int>(-1);
                lua_pop(L,1); // remove the weight

                cc.add( lib[sp_name],sp_weight );


                // remove the descriptor
                lua_pop(L,1);
            }
        }

        void __luaIO:: add(Boot::Loader        &loader,
                           Lua::State::Pointer &vm,
                           const string        &name,
                           Library             &lib )
        {
            lua_State *L = **vm;

            lua_settop(L,0);
            lua_getglobal(L,*name);
            if(!lua_istable(L,-1))
            {
                throw exception("no boot loader table '%s'", *name);
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
                __load_constraint(loader,vm, *name, lib, count);
                // remove value
                lua_pop(L,1);
            }
            // nil
            lua_pop(L,1);
        }
    }

}

