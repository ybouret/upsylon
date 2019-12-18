

#include "y/graphic/ops/filter/sobel.hpp"

namespace upsylon {

    namespace Graphic {

        Y_GRAPHIC_FILTER_IMPL(Sobel,X3,Point(-1,-1),Point(1,1))
        self[-1][-1] = -1; self[-1][1] = 1;
        self[ 0][-1] = -2; self[ 0][1] = 2;
        self[ 1][-1] = -1; self[ 1][1] = 1;
        Y_GRAPHIC_FILTER_DONE()


        Y_GRAPHIC_FILTER_IMPL(Sobel,Y3,Point(-1,-1),Point(1,1))
        self[-1][-1] = -1;
        self[-1][ 0] = -2;
        self[-1][ 1] = -1;

        self[1][-1] = 1;
        self[1][ 0] = 2;
        self[1][ 1] = 1;
        Y_GRAPHIC_FILTER_DONE()


        Y_GRAPHIC_FILTER_IMPL(Sobel,X5,Point(-2,-2),Point(2,2))
        self[-2][-2] =  -5;  self[-2][-1] =  -4; self[-2][1] =  4; self[-2][2] =  5;
        self[-1][-2] =  -8;  self[-1][-1] = -10; self[-1][1] = 10; self[-1][2] =  8;
        self[ 0][-2] = -10;  self[ 0][-1] = -20; self[ 0][1] = 20; self[ 0][2] = 10;
        self[ 1][-2] =  -8;  self[ 1][-1] = -10; self[ 1][1] = 10; self[ 1][2] =  8;
        self[ 2][-2] =  -5;  self[ 2][-1] =  -4; self[ 2][1] =  4; self[ 2][2] =  5;
        Y_GRAPHIC_FILTER_DONE()


        Y_GRAPHIC_FILTER_IMPL(Sobel,Y5,Point(-2,-2),Point(2,2))
        self[ 2][-2] =  5; self[ 2][-1] =   8; self[ 2][0] =  10; self[ 2][1] =   8; self[ 2][2] =  5;
        self[ 1][-2] =  4; self[ 1][-1] =  10; self[ 1][0] =  20; self[ 1][1] =  10; self[ 1][2] =  4;

        self[-1][-2] = -4; self[-1][-1] = -10; self[-1][0] = -20; self[-1][1] = -10; self[-1][2] = -4;
        self[-2][-2] = -5; self[-2][-1] =  -8; self[-2][0] = -10; self[-2][1] =  -8; self[-2][2] = -5;
        Y_GRAPHIC_FILTER_DONE()

    }

}

