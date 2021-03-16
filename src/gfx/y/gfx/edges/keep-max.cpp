
#include "y/gfx/edges/keep-max.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {

            keep_max:: keep_max(const unit_t W, const unit_t H) :
            histogram(),
            pixmap<uint8_t>(W,H)
            {
            }

            keep_max:: ~keep_max() throw()
            {
            }

            void keep_max:: operator()(broker &apply, gradient &G)
            {
                local_caches &caches = apply.caches;
                caches.make<size_t>( bins );

                struct ops
                {

                };


            }


        }

    }

}


