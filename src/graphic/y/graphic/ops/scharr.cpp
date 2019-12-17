#include "y/graphic/ops/scharr.hpp"

namespace upsylon {

    namespace Graphic {


        Y_GRAPHIC_FILTER_IMPL(Scharr,X3,Point(-1,-1),Point(1,1))
        self[-1][-1] = -1; self[-1][1] = 1;
        self[ 0][-1] = -2; self[ 0][1] = 3;
        self[ 1][-1] = -1; self[ 1][1] = 1;
        Y_GRAPHIC_FILTER_DONE()


        Y_GRAPHIC_FILTER_IMPL(Scharr,Y3,Point(-1,-1),Point(1,1))
        self[-1][-1] = -1;
        self[-1][ 0] = -3;
        self[-1][ 1] = -1;

        self[1][-1] = 1;
        self[1][ 0] = 3;
        self[1][ 1] = 1;
        Y_GRAPHIC_FILTER_DONE()


        Y_GRAPHIC_FILTER_IMPL(Scharr,X5,Point(-2,-2),Point(2,2))

        Y_GRAPHIC_FILTER_DONE()


        Y_GRAPHIC_FILTER_IMPL(Scharr,Y5,Point(-2,-2),Point(2,2))

        Y_GRAPHIC_FILTER_DONE()

    }

}

