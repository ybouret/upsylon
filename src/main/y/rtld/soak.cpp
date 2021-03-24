
#include "y/rtld/soak.hpp"
#include <cstdarg>

namespace upsylon
{
    void soak:: print(FILE *stream, const char *format, ...) throw()
    {
        if(stream)
        {
            va_list args;
            va_start (args, format);
            vfprintf (stream, format, args);
            va_end (args);
        }
    }
    
    const char soak:: unhandled_exception[] = "unhandled exception";
    bool       soak:: verbose = false;
}

