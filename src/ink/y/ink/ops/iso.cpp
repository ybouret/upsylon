#include "y/ink/ops/iso.hpp"

namespace upsylon
{
    namespace Ink
    {
        Iso:: ~Iso() throw() {}

        Iso:: Iso( const Area &area ) :
        Area(area),
        xy( area.w+area.h, 0.0 ),
        X( &xy[1] ),
        Y( X+area.w ),
	db(),
        levels()
        {
            double *p = &xy[1];
            for(size_t i=0;i<w;++i)
            {
                *(p++) = i;
            }
            for(size_t j=0;j<h;++j)
            {
                *(p++) = j;
            }
        }


    }
}
