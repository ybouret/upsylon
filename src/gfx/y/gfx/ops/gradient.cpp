
#include "y/gfx/ops/gradient.hpp"

namespace upsylon
{
    namespace graphic
    {
        gradient:: ~gradient() throw()
        {}
        
        
        gradient:: gradient(const unit_t W, const unit_t H) :
        pixmap<float>(W,H),
        g(W,H),
        gmax(0)
        {
        }
        
        
        
        void gradient:: compute(const pixmap<float> &f, const tile &t) throw()
        {
            assert(t.cache->is<float>());
            float lmax = 0;
            for(size_t j=t.size();j>0;--j)
            {
                const segment &s    = t[j];
                const unit_t   y    = s.y;
                const unit_t   xmin = s.xmin;
                pixmap<float> &gn   = *this;
                for(unit_t x=s.xmax;x>=xmin;--x)
                {
                    const float gx = f[y][x+1]-f[y][x-1];
                    const float gy = f[y+1][x]-f[y-1][x];
                    const float g2 = gx*gx + gy*gy;
                    if(g2>0)
                    {
                        const float gg = gn(y)(x) = sqrtf(g2);
                        lmax    = max_of(lmax,gg);
                        g(y)(x) = vertex(gx/gg,gy/gg);
                    }
                    else
                    {
                        gn(y)(x) = 0.0f;
                        g(y)(x)  = vertex(0.0f,0.0f);
                    }
                }
                
            }
            t.cache->as<float>() = lmax;
        }
        
        void gradient:: compute(const pixmap<float> &f, broker &apply)
        {
            apply.caches.make<float>();
            
            // compute gradient and max per tile
            {
                struct ops
                {
                    const pixmap<float> &f;
                    gradient            &G;
                    
                    static inline void run(const tile &t,
                                           void       *args,
                                           lockable   &) throw()
                    {
                        assert(args);
                        ops &self = *static_cast<ops *>(args);
                        self.G.compute(self.f,t);
                    }
                };
                
                ops todo = { f, *this };
                apply(ops::run,&todo);
                
            }
            
            // update global gmax
            aliasing::_(gmax) = apply.caches.get_max<float>();

        }
        
        namespace
        {
            static inline void normalize_kernel(const tile &t,
                                                void       *args,
                                                lockable   &) throw()
            {
                assert(args);
                gradient &G = *static_cast<gradient *>(args); assert(G.gmax>0.0f);
                for(size_t j=t.size();j>0;--j)
                {
                    const segment &s    = t[j];
                    const unit_t   y    = s.y;
                    const unit_t   xmin = s.xmin;
                    for(unit_t x=s.xmax;x>=xmin;--x)
                    {
                        G(y)(x) = clamp(0.0f,G(y)(x)/G.gmax,1.0f);
                    }
                }
            }
        }
        
        void gradient:: normalize(broker &apply) throw()
        {
            if(gmax>0.0f)
            {
                apply(normalize_kernel,this);
            }
        }

        
        
    }
    
}


