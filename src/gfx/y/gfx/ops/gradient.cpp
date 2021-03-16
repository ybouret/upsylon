
#include "y/gfx/ops/gradient.hpp"
#include "y/momentary/value.hpp"

namespace upsylon
{
    namespace graphic
    {
        gradient:: ~gradient() throw()
        {}
        
        
        gradient:: gradient(const unit_t W, const unit_t H, const shared_filters &F) :
        pixmap<float>(W,H),
        g(W,H),
        comp(F),
        gmax(0),
        host(NULL)
        {
        }
        
        
        
        void gradient:: compute_tile(const tile &t) throw()
        {
            assert(t.cache->is<float>());
            assert(host!=NULL);

            const pixmap<float> &f    = *host;
            float                lmax = 0;
            for(size_t j=t.size();j>0;--j)
            {
                const segment &s    = t[j];
                const unit_t   y    = s.y;
                const unit_t   xmin = s.xmin;
                pixmap<float> &norm = *this;
                for(unit_t x=s.xmax;x>=xmin;--x)
                {
                    float       gx = 0.0f;
                    float       gy = 0.0f;
                    {
                        const coord p(x,y);
                        comp->x->put(gx,f,p);
                        comp->y->put(gy,f,p);
                    }

                    const float    g2      = gx*gx + gy*gy;
                    pixrow<float>  &norm_y = norm(y);
                    pixrow<vertex> &grad_y = g(y);
                    if(g2>0.0f)
                    {
                        const float gg = norm_y(x) = sqrtf(g2);
                        lmax      = max_of(lmax,gg);
                        grad_y(x) = vertex(gx/gg,gy/gg);
                    }
                    else
                    {
                        norm_y(x)  = 0.0f;
                        grad_y(x)  = vertex(0.0f,0.0f);
                    }
                }
                
            }
            t.cache->as<float>() = lmax;
        }


        void gradient:: compute_call(const tile &t, void *args, lockable &) throw()
        {
            assert(args);
            static_cast<gradient *>(args)->compute_tile(t);
        }


        namespace
        {
            static inline void normalize_kernel(const tile &t,
                                                void       *args,
                                                lockable   &) throw()
            {
                assert(args);
                gradient    &G  = *static_cast<gradient *>(args); assert(G.gmax>0.0f);
                const float fac = 1.0f/G.gmax;
                for(size_t j=t.size();j>0;--j)
                {
                    const segment &s    = t[j];
                    const unit_t   xmin = s.xmin;
                    pixrow<float> &r    = G[s.y];
                    for(unit_t x=s.xmax;x>=xmin;--x)
                    {
                        r(x) = clamp(0.0f,fac*r(x),1.0f);
                    }
                }
            }
        }

        
        void gradient:: compute(const pixmap<float> &f, broker &apply)
        {
            //------------------------------------------------------------------
            // prepare memory
            //------------------------------------------------------------------
            apply.caches.make<float>();

            //------------------------------------------------------------------
            // compute per tile, store local gmax
            //------------------------------------------------------------------
            {
                momentary_value< const pixmap<float> * > temp(host,&f);
                apply(compute_call,this);
            }
            
            //------------------------------------------------------------------
            // update global gmax
            //------------------------------------------------------------------
            aliasing::_(gmax) = apply.caches.get_max<float>();

            //------------------------------------------------------------------
            // normalize
            //------------------------------------------------------------------
            apply(normalize_kernel,this);
            
        }



#if 0
        namespace
        {
            static inline void maxima_kernel(const tile &t,
                                             void       *args,
                                             lockable   &) throw()
            {
                assert(args);
                gradient                 &G = *static_cast<gradient *>(args); assert(G.gmax>0.0f);
                pixmap<gradient::vertex> &V = G.g;
                
                for(size_t j=t.size();j>0;--j)
                {
                    const segment  &s    = t[j];
                    const unit_t    y    = s.y;
                    const unit_t    xmin = s.xmin;
                    pixrow<float>            &Gy = G(y);
                    pixrow<gradient::vertex> &Vy = V(y);
                    
                    for(unit_t x=s.xmax;x>=xmin;--x)
                    {
                        float             &G0  = Gy(x);
                        gradient::vertex  &gv  = Vy(x);
                        const unit_t       dx  =  unit_t( floorf(gv.x+0.5f) );
                        const unit_t       dy  =  unit_t( floorf(gv.y+0.5f) );
                        const float        Gm  = G[y-dy][x-dx];
                        const float        Gp  = G[y+dy][x+dx];
                        if(G0<Gp||G0<Gm)
                        {
                            G0 = 0.0f;
                        }
                    }
                }
            }
        }

#endif

        void gradient:: maxima(broker &apply) throw()
        {
            //apply(maxima_kernel,this);
        }
        
    }
    
}


