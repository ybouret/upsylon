

//! \file

#ifndef Y_GFX_EDGES_KEEP_MAX_INCLUDED
#define Y_GFX_EDGES_KEEP_MAX_INCLUDED 1

#include "y/gfx/edges/gradient.hpp"
#include "y/gfx/color/histogram.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {
            class keep_max :
            public histogram,
            public pixmap<uint8_t>
            {
            public:
                explicit keep_max(const unit_t W, const unit_t H);
                virtual ~keep_max() throw();

                void operator()(broker &apply, gradient &G);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(keep_max);
            };
        }
    }
}

#endif
