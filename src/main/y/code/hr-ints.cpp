
#include "y/code/hr-ints.hpp"
#include "y/type/utils.hpp"
#include "y/core/ipower.hpp"
#include <iostream>

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
            uint64_t v = ipower(k,j);
            while(v>u)
            {
                --j;
                v >>= 10;
            }
            //std::cerr << "u=" << u <<  ", v=" << v << ", j=" << j << std::endl;
            (double &)value = s*(double(u)/double(v));
            (char   &)radix = rx[j];
        }
    }

    human_readable:: human_readable( const human_readable &hr ) throw() :
    value(hr.value),
    radix(hr.radix)
    {
    }

    human_readable:: ~human_readable() throw()
    {
        (double&)value = 0;
        (char  &)radix = 0;
    }

    human_readable & human_readable:: operator=( const human_readable &hr ) throw()
    {
        (double &)value = hr.value;
        (char   &)radix = hr.radix;
        return *this;
    }

}
