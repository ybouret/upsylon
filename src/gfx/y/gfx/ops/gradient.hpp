//! \file

#ifndef Y_GFX_OPS_GRAD_INCLUDED
#define Y_GFX_OPS_GRAD_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/pixel.hpp"
#include "y/gfx/color/type-to-rgba.hpp"

namespace upsylon
{
    namespace graphic
    {
      
        class gradient : public pixmap<float>
        {
        public:
            typedef point2d<float> vertex;
            
            template <size_t INDX>
            class vtx_to_rgba : public type_to_rgba
            {
            public:
                const rgb lower; //!< mapping -1
                const rgb upper; //!< mapping  1
                inline explicit vtx_to_rgba(const rgb cold, const rgb hot) throw() :
                type_to_rgba(), lower(cold), upper(hot)
                {}
                
                inline virtual ~vtx_to_rgba() throw() {}
                
                inline virtual unit_t depth() const throw() { return sizeof(vertex); }
                
                inline virtual rgba operator()(const void *addr) const throw()
                {
                    assert(addr);
                    const float upper_weight = clamp(0.0f, 0.5f*(1.0f+*(static_cast<const float *>(addr)+INDX)), 1.0f );
                    const float lower_weight = 1.0f-upper_weight;
                    const float r = float(lower.r) * lower_weight + float(upper.r) * upper_weight;
                    const float g = float(lower.g) * lower_weight + float(upper.g) * upper_weight;
                    const float b = float(lower.b) * lower_weight + float(upper.b) * upper_weight;
                    return rgb(uint8_t( floorf(r+0.5f) ),
                               uint8_t( floorf(g+0.5f) ),
                               uint8_t( floorf(b+0.5f) ) );
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(vtx_to_rgba);
            };
            
            typedef vtx_to_rgba<0> x_to_rgba;
            typedef vtx_to_rgba<1> y_to_rgba;
            
            
            
            pixmap<vertex>         g;    //!< map of normalized vertices
            float                  gmax; //!< last max gradient
            
            explicit gradient(const unit_t W, const unit_t H);
            virtual ~gradient() throw();
            
            void compute(const pixmap<float> &, broker &apply);
            void normalize(broker &apply) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(gradient);
            void compute(const pixmap<float> &, const tile &t) throw();
        };
        
    }
    
}

#endif
