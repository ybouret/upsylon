
//! \file

#ifndef Y_GFX_EDGES_GRADIENT_INCLUDED
#define Y_GFX_EDGES_GRADIENT_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/filters.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {
            class gradient : public pixmap<float>
            {
            public:
                typedef point2d<int8_t> vertex;

                pixmap<vertex> probe;

                explicit gradient(const unit_t W, const unit_t H);
                virtual ~gradient() throw();

                float compute(const pixmap<float>  &field,
                              broker               &apply,
                              const shared_filters &delta);
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(gradient);
            };
        }
    }

}

#endif
