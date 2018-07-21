#include "y/parallel.hpp"
#include <cstring>
#include <cstdio>
#include <iostream>
namespace upsylon
{
    void parallel:: __format() throw()
    {
        static const size_t max_size = 999;
        char *s = (char *)label;
        memset(s,0,sizeof(label));
        if(size>max_size)
        {
            strncpy(s,"too big",sizeof(label));
        }
        else
        {
            const char *fmt = "%03u.%03u";
            if(size<100)
            {
                fmt = "%02u.%02u";
            }
            if(size<10)
            {
                fmt = "%u.%u";
            }
            snprintf(s,sizeof(label),fmt,unsigned(size),unsigned(rank));
        }
    }

    void parallel:: free() throw()
    {
        if(bytes>0)
        {
            assert(space);
            object::operator delete(space,bytes);
            bytes = 0;
            space = 0;
        }
    }

    void parallel:: make( const size_t n )
    {
        if(n>bytes)
        {
            free();
            space = object::operator new(n);
            bytes = n;
        }
        else
        {
            memset(space,0,bytes);
        }
    }
}
