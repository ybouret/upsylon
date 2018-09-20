#include "y/lua++/vm.hpp"
#include <cstdlib>

namespace upsylon
{

    namespace Lua
    {
        static const char fn[] = "Lua::State: ";

        void * __luaAllocator(void *,
                              void  *ptr,
                              size_t ,//osize,
                              size_t nsize)
        {
            Y_LOCK(memory::global::access);
            if (nsize == 0)
            {
                free(ptr);
                return NULL;
            }
            else
            {
                return realloc(ptr, nsize);
            }
        }

        State:: State() : L( lua_newstate(__luaAllocator,NULL) )
        {
            luaL_openlibs(L);
        }

        State:: ~State() throw()
        {
            lua_close(L);
            L=0;
        }

        void State:: doFile( const string &filename )
        {
            Y_LUA_CHECK(luaL_dofile(L,*filename));
        }

        void State:: doString( const string &code )
        {
            Y_LUA_CHECK(luaL_dostring(L,*code));
        }

        template <>
        double State:: get<double>( const string &id )
        {
            lua_settop(L,0);
            lua_getglobal(L,*id);
            if( !lua_isnumber(L,-1) )
            {
                throw exception("%sno double '%s'",fn,*id);
            }
            return double( lua_tonumber(L,-1) );
        }


        template <>
        float State:: get<float>( const string &id )
        {
            lua_settop(L,0);
            lua_getglobal(L,*id);
            if( !lua_isnumber(L,-1) )
            {
                throw exception("%sno float '%s'",fn,*id);
            }
            return float( lua_tonumber(L,-1) );
        }

        template <>
        int State:: get<int>( const string &id )
        {
            lua_settop(L,0);
            lua_getglobal(L,*id);
            if( !lua_isinteger(L,-1) )
            {
                throw exception("%sno int '%s'",fn,*id);
            }
            return int( lua_tointeger(L,-1) );
        }

        template <>
        size_t State:: get<size_t>( const string &id )
        {
            lua_settop(L,0);
            lua_getglobal(L,*id);
            if( !lua_isinteger(L,-1) )
            {
                throw exception("%sno size_t '%s'",fn,*id);
            }
            const int i = lua_tointeger(L,-1);
            if(i<0) throw exception("%s'%s' is negative (%d) for size_t",fn,*id,i);
            return size_t(i);
        }

        template <>
        string State:: get<string>( const string &id )
        {
            lua_settop(L,-1);
            lua_getglobal(L,*id);
            if( !lua_isstring(L,-1) )
            {
                throw exception("%sno string '%s'",fn,*id);
            }
            size_t      len = 0;
            const char *buf = lua_tolstring(L,-1,&len);
            return string(buf,len);
        }

    }

}
