
#include "y/graphic/ops/filter/prewitt.hpp"

namespace upsylon {

    namespace Graphic {


        Y_GRAPHIC_FILTER_IMPL(Prewitt,X3,Point(-1,-1),Point(1,1))
        self[-1][-1] = -1; self[-1][1] = 1;
        self[ 0][-1] = -1; self[ 0][1] = 1;
        self[ 1][-1] = -1; self[ 1][1] = 1;
        Y_GRAPHIC_FILTER_DONE()


        Y_GRAPHIC_FILTER_IMPL(Prewitt,Y3,Point(-1,-1),Point(1,1))
        self[-1][-1] = -1;
        self[-1][ 0] = -1;
        self[-1][ 1] = -1;

        self[1][-1] = 1;
        self[1][ 0] = 1;
        self[1][ 1] = 1;

        Y_GRAPHIC_FILTER_DONE()


        Y_GRAPHIC_FILTER_IMPL(Prewitt,X5,Point(-2,-2),Point(2,2))
        self[-2][-2] = -2;  self[-2][-1] = -1; self[-2][1] = 1; self[-2][2] = 2;
        self[-1][-2] = -2;  self[-1][-1] = -1; self[-1][1] = 1; self[-1][2] = 2;
        self[ 0][-2] = -2;  self[ 0][-1] = -1; self[ 0][1] = 1; self[ 0][2] = 2;
        self[ 1][-2] = -2;  self[ 1][-1] = -1; self[ 1][1] = 1; self[ 1][2] = 2;
        self[ 2][-2] = -2;  self[ 2][-1] = -1; self[ 2][1] = 1; self[ 2][2] = 2;
        Y_GRAPHIC_FILTER_DONE()


        Y_GRAPHIC_FILTER_IMPL(Prewitt,Y5,Point(-2,-2),Point(2,2))
        self[ 2][-2] =  2; self[ 2][-1] =  2; self[ 2][0] =  2; self[ 2][1] =  2; self[ 2][2] =  2;
        self[ 1][-2] =  1; self[ 1][-1] =  1; self[ 1][0] =  1; self[ 1][1] =  1; self[ 1][2] =  1;

        self[-1][-2] = -1; self[-1][-1] = -1; self[-1][0] = -2; self[-1][1] = -1; self[-1][2] = -1;
        self[-2][-2] = -2; self[-2][-1] = -2; self[-2][0] = -1; self[-2][1] = -2; self[-2][2] = -2;
        Y_GRAPHIC_FILTER_DONE()
    }

}

