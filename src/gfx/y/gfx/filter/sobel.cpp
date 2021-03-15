
#include "y/gfx/filter/sobel.hpp"

namespace upsylon
{
    
    namespace graphic
    {
        const char Sobel3:: ID[] = "sobel3";
        
        static const int8_t coeff[3][3] =
        {
            {-1,-2,-1},
            {0,0,0},
            {1,2,1}
        };
        
        Sobel3:: ~Sobel3() throw() {}
        
        Sobel3:: Sobel3() :
        filters(ID, &coeff[0][0],3)
        {
        }
        
        
    }
    
}
