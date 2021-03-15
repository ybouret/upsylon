
#include "y/gfx/filter/prewitt.hpp"

namespace upsylon
{
    
    namespace graphic
    {
        Y_FILTERS_IMPL(Prewitt,3)
        
        {-1,-1,-1},
        {0,0,0},
        {1,1,1}
        
        Y_FILTERS_DONE();
        
        
        
    }
    
}

namespace upsylon
{
    
    namespace graphic
    {
        Y_FILTERS_IMPL(Prewitt,5)
        
        { -2, -2, -2,  -2, -2 },
        { -1, -1, -1,  -1, -1 },
        {  0,  0,  0,   0,  0 },
        {  2,  2,  2,   2,  2 },
        {  1,  1,  1,   1,  1 },
        
        Y_FILTERS_DONE();
        
    }
    
}

namespace upsylon
{
    
    namespace graphic
    {
        Y_FILTERS_IMPL(Prewitt,7)
        
        { -3,-3,-3,-3,-3,-3,-3 },
        { -2,-2,-2,-2,-2,-2,-2 },
        { -1,-1,-1,-1,-1,-1,-1 },
        {  0, 0, 0, 0, 0, 0, 0 },
        {  1, 1, 1, 1, 1, 1, 1 },
        {  2, 2, 2, 2, 2, 2, 2 },
        {  3, 3, 3, 3, 3, 3, 3 }
        
        Y_FILTERS_DONE();
        
    }
    
}
