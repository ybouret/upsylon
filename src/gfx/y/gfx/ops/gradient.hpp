//! \file

#ifndef Y_GFX_OPS_GRAD_INCLUDED
#define Y_GFX_OPS_GRAD_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/pixel.hpp"

namespace upsylon
{
    namespace graphic
    {
      
        class gradient : public pixmap<float>
        {
        public:
            typedef point2d<float> vertex;
            pixmap<vertex> g;
            
            explicit gradient(const unit_t W, const unit_t H);
            virtual ~gradient() throw();
            
            void compute(const pixmap<float> &, broker &apply);
            void compute(const pixmap<float> &, const tile &t) throw();
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(gradient);
        };
        
    }
    
}

#endif
