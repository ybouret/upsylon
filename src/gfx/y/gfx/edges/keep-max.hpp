

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
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! setup
                explicit keep_max(const unit_t W, const unit_t H);
                
                //! cleanup
                virtual ~keep_max() throw();

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! apply algorithm
                /**
                 - check local maxima in gradient direction
                 - convert it to byte
                 - produce the histogram of local extrema
                 */
                void operator()(broker &apply, const gradient &G);

                //! rebuild histogram
                void update(broker &apply);

                //! close current field
                void close(broker &apply) throw();

                //! open current field
                void open(broker &apply) throw();

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                pixmap<uint8_t> aux; //!< auxiliary field

            private:
                Y_DISABLE_COPY_AND_ASSIGN(keep_max);
                local_caches &setup_with(broker &apply);
                void          merge_with(const local_caches &) throw();
            };
        }
    }
}

#endif
