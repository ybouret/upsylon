
#include "y/gfx/filters/sobel.hpp"

namespace upsylon
{
    
    namespace graphic
    {
        Y_FILTERS_IMPL(Sobel,3)
        
        {-1,-2,-1},
        {0,0,0},
        {1,2,1}
        
        Y_FILTERS_DONE();
        
        
        
    }
    
}

namespace upsylon
{
    
    namespace graphic
    {
        Y_FILTERS_IMPL(Sobel,5)
        
        { -5,  -8, -10,  -8, -5 },
        { -4, -10, -20, -10, -4 },
        {  0,   0,   0,   0,  0 },
        {  4,  10,  20,  10,  4 },
        {  5,   8,  10,   8,  5 },
        
        Y_FILTERS_DONE();
        
    }
    
}

namespace upsylon
{
    
    namespace graphic
    {
        Y_FILTERS_IMPL(Sobel,7)
        { -780, -1080, -1404, -1080, -1404, -1080, -780 },
        { -720, -1170, -1872, -2430, -1872, -1170, -720 },
        { -468,  -936, -2340, -4680, -2340,  -936, -468 },
        { 0, 0, 0, 0, 0, 0, 0 },
        { 468,  936, 2340, 4680, 2340,  936, 468 },
        { 720, 1170, 1872, 2430, 1872, 1170, 720 },
        { 780, 1080, 1404, 1080, 1404, 1080, 780 }
        Y_FILTERS_DONE();
        
    }
    
}
