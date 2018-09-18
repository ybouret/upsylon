#include "y/lua++/vm.hpp"


namespace upsylon
{

    namespace Lua
    {
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
    }

}
