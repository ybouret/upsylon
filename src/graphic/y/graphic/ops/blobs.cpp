
#include "y/graphic/ops/blobs.hpp"

namespace upsylon {

    namespace Graphic {

        Blob:: Blob( const size_t blobLabel ) throw() : label(blobLabel) {}
        Blob:: ~Blob() throw()
        {
        }
        

        const Point Blobs::delta[8] =
        {
            Point(-1,0),
            Point(1,0),
            Point(0,-1),
            Point(0,1),

            Point(1,1),
            Point(1,-1),
            Point(-1,-1),
            Point(-1,1)

        };

        Blobs:: Blobs( const size_t W, const size_t  H) :
        Pixmap<size_t>(W,H),
        Blob::List()
        {
        }

        Blobs:: ~Blobs() throw()
        {
            
        }
        
    }

}
