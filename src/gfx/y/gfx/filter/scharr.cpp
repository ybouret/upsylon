
#include "y/gfx/filter/scharr.hpp"

namespace upsylon
{
    
    namespace graphic
    {
        Y_FILTERS_IMPL(Scharr,3)
        
        {-3,-10, -3 },
        { 0,  0,  0 },
        { 3, 10,  3 }
        
        Y_FILTERS_DONE();
        
        
        
    }
    
}

namespace upsylon
{
    
    namespace graphic
    {
        Y_FILTERS_IMPL(Scharr,5)
        
        { -1, -2, -6, -2, -1 },
        { -1, -2, -3, -2, -1 },
        {  0,  0,  0,  0,  0 },
        {  1,  2,  3,  2,  1 },
        {  1,  2,  6,  2,  1 },
        
        Y_FILTERS_DONE();
        
    }
    
}
 
