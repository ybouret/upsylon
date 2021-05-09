#ifndef Y_FIND_EXE_INCLUDED
#define Y_FIND_EXE_INCLUDED 1

#include "y/string.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/allocator/pooled.hpp"

namespace upsylon
{
    
    class exe_paths : public accessible<const string>
    {
    public:
        explicit exe_paths() throw();
        virtual ~exe_paths() throw();
        
        
        virtual size_t        size()                   const throw();
        virtual const string &operator[](const size_t) const throw();
     
        bool   add(const string &d);
        size_t load(const char *path_env_name);
        
    private:
        typedef vector<string,memory::pooled> strings;
        strings dirs;
    };
    
    
}

#endif

