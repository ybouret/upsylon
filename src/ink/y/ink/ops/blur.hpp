//! \file
#ifndef Y_INK_BLUR_INCLUDED
#define Y_INL_BLUR_INCLUDED 1

#include "y/ink/pixmap.hpp"

namespace upsylon
{
    namespace Ink
    {

        class Blur : public Object
        {
        public:
            virtual ~Blur() throw();
            explicit Blur(const float sig);
            const float   sigma;   //!< gaussian deviation
            const float   scale;   //!< 2*sigma^2
            const unit_t  length;  //!< length >= sqrt(2*sigma^2*log(256))
            vector<float> weight;  //!< length+1
            
        private:
            Y_DISABLE_ASSIGN(Blur);
        };

    }
}

#endif

