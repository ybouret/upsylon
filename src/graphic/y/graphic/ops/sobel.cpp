

#include "y/graphic/ops/sobel.hpp"

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

        Y_GRAPHIC_FILTER_DONE()


        Y_GRAPHIC_FILTER_IMPL(Sobel,Y5,Point(-2,-2),Point(2,2))

        Y_GRAPHIC_FILTER_DONE()

    }

}

