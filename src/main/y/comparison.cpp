
#include "y/comparison.hpp"

namespace upsylon
{
    int comparison:: increasing_addresses(const void *lhs, const void *rhs) throw()
    {
        if(lhs<rhs)
        {
            return -1;
        }
        else
        {
            if(rhs<lhs)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
    
    int comparison:: decreasing_addresses(const void *lhs, const void *rhs) throw()
    {
        if(lhs<rhs)
        {
            return 1;
        }
        else
        {
            if(rhs<lhs)
            {
                return -1;
            }
            else
            {
                return 0;
            }
        }
    }

}
