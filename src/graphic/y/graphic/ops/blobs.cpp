
#include "y/graphic/ops/blobs.hpp"

namespace upsylon {

    namespace Graphic {

        Blob:: Blob( const size_t blobLabel ) throw() : label(blobLabel) {}
        Blob:: ~Blob() throw()
        {
        }
        

      

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
