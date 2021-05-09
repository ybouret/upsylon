#ifndef Y_FIND_EXE_INCLUDED
#define Y_FIND_EXE_INCLUDED 1

#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/pooled.hpp"

namespace upsylon
{
    struct find_exe
    {
        typedef vector<string,memory::pooled> strings;
        static size_t load_paths(strings &paths, const char *path_env_name);
    };
}

#endif

