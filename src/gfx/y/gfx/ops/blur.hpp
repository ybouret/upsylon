//! \file

#ifndef Y_GFX_BLUR_INCLUDED
#define Y_GFX_BLUR_INCLUDED 1

#include "y/gfx/pixmap.hpp"

namespace upsylon
{
    namespace graphic
    {
        

        class blur
        {
        public:
            static const float expand; //!< 2*sqrt(2*log(2))




            static unit_t r_max_for(const float sig) throw();

            explicit blur(const float sig);
            virtual ~blur() throw();


            const unit_t        r_max;  //!< rmax_for(sigma)
            const pixmap<float> weight; //! (1+rmax,1+rmax)
            const float         factor; //!< normalize factor
            const float         sigma;  //!< std dev
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(blur);
        };

    }



}

#endif

