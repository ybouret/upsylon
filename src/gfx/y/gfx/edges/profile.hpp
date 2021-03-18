

//! \file

#ifndef Y_GFX_EDGES_PROFILE_INCLUDED
#define Y_GFX_EDGES_PROFILE_INCLUDED 1

#include "y/gfx/ops/blobs.hpp"

namespace upsylon
{
    namespace graphic
    {

        
#define Y_GFX_EDGE_VACANT 0       //!< vacant edge
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
            class profile : public pixmap<size_t>
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                explicit profile(const unit_t W, const unit_t H); //!< setup
                virtual ~profile() throw();                       //!< cleanup

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                //! tighten edges values to STRING/FEEBLE/VACANT
                /**
                 \return the number of active edges
                 */
                static size_t tighten(pixmap<uint8_t> &edges,
                                      broker          &apply,
                                      const uint8_t    feeble_limit,
                                      const uint8_t    strong_limit);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(profile);
            };

        }

    }

}

#endif
