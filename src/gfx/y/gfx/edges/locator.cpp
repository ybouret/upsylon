
#include "y/gfx/edges/locator.hpp"
#include "y/gfx/filters/db.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {




            locator:: ~locator() throw()
            {
            }

            
            locator:: locator(const string       &F,
                              const unit_t        W,
                              const unit_t        H,
                              const shared_knots &K):
            detector(W,H),
            delta( filters_db::instance()[F] ),
            cache( K )
            {
            }


            locator:: locator(const char         *F,
                              const unit_t        W,
                              const unit_t        H,
                              const shared_knots &K):
            detector(W,H),
            delta( filters_db::instance()[F] ),
            cache( K )
            {
            }

            size_t locator:: finalize(blobs &userBlobs, const size_t cutSize)
            {
                (void) extract(userBlobs,cache);
                userBlobs.remove_below(cutSize,*this);
                return userBlobs.size;
            }

            size_t locator:: direct_find(blobs                &userBlobs,
                                         const pixmap<float>  &source,
                                         broker               &apply,
                                         const blur           *cloud,
                                         const size_t          cutSize)
            {
                (void) prepare(source,apply,delta,cloud);
                return finalize(userBlobs,cutSize);
            }

        }

    }

}

