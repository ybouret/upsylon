
#include "y/graphic/ops/prewitt.hpp"

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
        

        Y_GRAPHIC_FILTER_DONE()


        Y_GRAPHIC_FILTER_IMPL(Prewitt,Y5,Point(-2,-2),Point(2,2))

        Y_GRAPHIC_FILTER_DONE()
    }

}

