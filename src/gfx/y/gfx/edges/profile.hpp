

//! \file

#ifndef Y_GFX_EDGES_PROFILE_INCLUDED
#define Y_GFX_EDGES_PROFILE_INCLUDED 1

#include "y/gfx/edges/keep-max.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {

            
            class profile : public pixmap<size_t>
            {
            public:
                explicit profile(const unit_t W, const unit_t H);
                virtual ~profile() throw();

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
