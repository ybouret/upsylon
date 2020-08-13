
#include "y/code/hr-ints.hpp"
#include "y/core/ipower.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"
#include <iostream>
#include <cstdio>

namespace upsylon {
    
    static const char rx[] = { 'b', 'k', 'M', 'G', 'T', 'E', 'Z' };
    
    human_readable:: human_readable( int64_t i ) throw() :
    value(0),
    radix(rx[0])
    {
        
        if(i!=0)
        {
            uint64_t u = i;
            int      s = 1;
            if(i<0)
            {
                u = -i;
                s = -1;
            }
            // u=1024^j+r
            static const uint64_t k = 1024;
            size_t   j = 6;
            uint64_t v = ipower<uint64_t>(k,j);
            while(v>u)
            {
                --j;
                v >>= 10;
            }
            aliasing::_(value) = s*(double(u)/double(v));
            aliasing::_(radix) = rx[j];
        }
    }
    
    human_readable:: human_readable( const human_readable &hr ) throw() :
    value(hr.value),
    radix(hr.radix)
    {
    }
    
    human_readable:: ~human_readable() throw()
    {
        _bzset(value);
        _bzset(radix);
    }
    
    human_readable & human_readable:: operator=( const human_readable &hr ) throw()
    {
        aliasing::_(value) = hr.value;
        aliasing::_(radix) = hr.radix;
        return *this;
    }
    
    std::ostream & operator<<( std::ostream &os, const human_readable &hr)
    {
        char buffer[128] = { 0 };
        Y_BZSET_STATIC(buffer);
        snprintf(buffer, sizeof(buffer)-1, "%8.2f", hr.value);
        os << &buffer[0] << hr.radix;
        return os;
    }
}
