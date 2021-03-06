

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
            //__________________________________________________________________
            //
            //
            //! keep only local maxima of a normalized gradient field
            //
            //__________________________________________________________________
            class keep_max : public histogram, public pixmap<uint8_t>
            {
            public:
                
                //! setup
                explicit keep_max(const unit_t W, const unit_t H);
                
                //! cleanup
                virtual ~keep_max() throw();

                //! apply algorithm
                /**
                 - check local maxima in gradient direction
                 - convert it to byte
                 - produce the histogram of local extrema
                 */
                void operator()(broker &apply, gradient &G);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(keep_max);
            };
        }
    }
}

#endif
