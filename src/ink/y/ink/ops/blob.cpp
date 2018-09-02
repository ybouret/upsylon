#include "y/ink/ops/blob.hpp"

namespace upsylon
{
    namespace Ink
    {

        Blobs:: ~Blobs() throw()
        {
        }

        Blobs:: Blobs(const size_t W, const size_t H) :
        __Blobs(W,H),
        count(0),
        vpool()
        {
        }
        


    }

}

