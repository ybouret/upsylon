
#include "y/graphic/metrics.hpp"
#include "y/exception.hpp"


namespace upsylon {

    namespace Graphic {

        Metrics:: ~Metrics() throw()
        {
        }

        static inline size_t checkDepth(const size_t D )
        {
            if(D<=0) throw exception("Graphic::Metrics(zero depth)");
            return D;
        }

        Metrics:: Metrics( const size_t W, const size_t H, const size_t D ) :
        Area(W,H),
        depth( checkDepth(D) ),
        scanline( depth * w ),
        stride(   scanline  )
        {
        }


        Metrics:: Metrics(const size_t W,
                          const size_t H,
                          const size_t D,
                          const size_t S) :
        Area(W,H),
        depth( checkDepth(D) ),
        scanline( depth * w ),
        stride( S  )
        {
            if(stride<scanline) throw exception("Graphic::Metrics(stride is too small)");
        }


    }

}



