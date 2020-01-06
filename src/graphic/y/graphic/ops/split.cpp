
#include "y/graphic/ops/split.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Graphic {

        void Split:: checkChannels(const accessible<size_t> &channels,
                                   const size_t              dims )
        {
            assert(dims>0);
            for(size_t i=channels.size();i>0;--i)
            {
                const size_t j = channels[i];
                if(j>=dims)
                {
                    throw exception("Graphic::Split: invalid channel[%u]=%u not in [0..%u-1]", unsigned(i), unsigned(j), unsigned(dims));
                }
            }
        }


    }
}

