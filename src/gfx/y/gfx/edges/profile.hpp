//! \file

#ifndef Y_GFX_EDGES_PROFILE_INCLUDED
#define Y_GFX_EDGES_PROFILE_INCLUDED 1

#include "y/gfx/ops/blobs.hpp"

namespace upsylon
{
    namespace graphic
    {

        
#define Y_GFX_EDGE_FEEBLE 127     //!< feeble edge
#define Y_GFX_EDGE_STRONG 255     //!< strong edge
        
        namespace edges
        {
            
            
            //__________________________________________________________________
            //
            //
            //! profile edges
            //
            //__________________________________________________________________
            class profile
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit profile() throw();       //!< setup
                virtual ~profile() throw();       //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                //! tighten edges values to STRONG/FEEBLE 
                /**
                 \return the number of active edges
                 */
                static size_t tighten(pixmap<uint8_t> &edges,
                                      broker          &apply,
                                      const uint8_t    feeble_limit,
                                      const uint8_t    strong_limit);

#if 0
                //! track edges with a least one STRONG component
                size_t track(blobs           &userBlobs,
                             pixmap<uint8_t> &userEdges,
                             pixmap<size_t>  &userMasks,
                             shared_knots    &knotCache);
#endif
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(profile);
                //blobs           *Blobs;
                pixmap<uint8_t> *Edges;
                pixmap<size_t>  *Masks;
                
                bool        accept(const blob &b)            throw();
                static bool call_accept(const blob &,void *) throw();
                
            };

        }

    }

}

#endif
