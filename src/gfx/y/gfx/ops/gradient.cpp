
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
        gdir(W,H),
        edge(W,H),
        comp(F),
        gmax(0),
        strong(0),
        feeble(0),
        host(NULL)
        {
        }
        
        
        
        void gradient:: compute(const tile &t) throw()
        {
            // sanity check
            assert(t.cache->is<float>());
            assert(host!=NULL);

            // initialize local data
            const pixmap<float> &f    = *host;
            float                lmax = 0;

            for(size_t j=t.size();j>0;--j)
            {
                const segment  &s    = t[j];
                const unit_t    y    = s.y;
                const unit_t    xmin = s.xmin;
                pixmap<float>  &norm = *this;
                pixmap<vertex> &grad = gdir;

                for(unit_t x=s.xmax;x>=xmin;--x)
                {
                    float       gx = 0.0f;
                    float       gy = 0.0f;
                    {
                        const coord p(x,y);
                        comp->x->put(gx,f,p);
                        comp->y->put(gy,f,p);
                    }

                    const float    g2      = (gx*gx) + (gy*gy);
                    pixrow<float>  &norm_y = norm(y);
                    pixrow<vertex> &grad_y = grad(y);
                    if(g2>0.0f)
                    {
                        const float gg = sqrtf(g2);    if(gg>lmax) lmax = gg;
                        norm_y(x)      = gg;
                        grad_y(x)      = vertex( int8_t( floorf(gx/gg+0.5f) ), int8_t( floorf(gy/gg+0.5f) ) );
                    }
                    else
                    {
                        norm_y(x)  = 0.0f;
                        grad_y(x)  = vertex(0,0);
                    }
                }
                
            }
            t.cache->as<float>() = lmax;
        }


        void gradient:: compute(const tile &t, void *args, lockable &) throw()
        {
            assert(args);
            static_cast<gradient *>(args)->compute(t);
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

        
        void gradient:: compute(broker &apply, const pixmap<float> &f)
        {
            //------------------------------------------------------------------
            // prepare memory
            //------------------------------------------------------------------
            static const size_t hbytes = histogram::bins * sizeof(size_t);
            static const size_t floats = hbytes/sizeof(float);
            apply.caches.make<float>(floats);

            //------------------------------------------------------------------
            // compute per tile, store local gmax
            //------------------------------------------------------------------
            {
                momentary_value< const pixmap<float> * > temp(host,&f);
                apply(compute,static_cast<gradient *>(this));
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


        void gradient:: keepmax(const tile &t) throw()
        {
            assert( t.cache->is<size_t>()  );
            assert( t.cache->tell()  >= 256 );
            assert( t.cache->length()>= 256*sizeof(size_t) );

            size_t *h = & t.cache->as<size_t>();
#if 0
            for(size_t i=0;i<256;++i) { assert(0==h[i]); }
#endif

            const pixmap<float> &G = *this;
            pixmap<uint8_t>     &E = edge;

            for(size_t j=t.size();j>0;--j)
            {
                const segment       &s    = t[j];
                const unit_t         y    = s.y;
                const unit_t         xmin = s.xmin;
                const pixrow<float> &G_y  = (*this)(y);
                pixrow<vertex>      &d_y  = gdir(y);
                pixrow<uint8_t>     &E_y  = E(y);

                for(unit_t x=s.xmax;x>=xmin;--x)
                {
                    const float        G0    = G_y(x);
                    vertex            &delta = d_y(x);
                    const unit_t       dx    = delta.x;
                    const unit_t       dy    = delta.y;
                    const float        Gm    = G[y-dy][x-dx];
                    const float        Gp    = G[y+dy][x+dx];
                    if(G0<Gp||G0<Gm)
                    {
                        E_y(x) = 0;
                        delta  = vertex(0,0);
                    }
                    else
                    {
                        ++h[ E_y(x) =  crux::convert::to_byte(G0) ];
                    }
                }
            }
        }

        void gradient:: keepmax(const tile &t, void *args, lockable &) throw()
        {
            assert(args);
            static_cast<gradient *>(args)->keepmax(t);
        }




        void gradient:: keepmax(broker &apply, histogram &H) 
        {
            //------------------------------------------------------------------
            // prepare local caches
            //------------------------------------------------------------------
            local_caches &caches = apply.caches;
            caches.make<size_t>( histogram::bins );

            //------------------------------------------------------------------
            // call function per tile
            //------------------------------------------------------------------
            apply(keepmax,static_cast<gradient *>(this));

            //------------------------------------------------------------------=
            // reduce histogram
            //------------------------------------------------------------------
            H.reset();
            const size_t nc = caches.size();
            for(size_t i=0;i<nc;++i)
            {
                const size_t * h = & (caches[i]->as<size_t>());
                H.add(h);
            }

        }

        void gradient:: profile(broker       &apply,
                                const uint8_t strong_threshold,
                                const uint8_t feeble_threshold)
        {
            assert(strong_threshold>=feeble_threshold);
            aliasing::_(feeble) = feeble_threshold;
            aliasing::_(strong) = strong_threshold;
            std::cerr << "profile with: " << int(feeble) << "->" << int(strong) << std::endl;
            apply(profile,static_cast<gradient *>(this));
        }


        void gradient:: profile(const tile &t) throw()
        {
            pixmap<uint8_t> &E = edge;

            const unsigned lo = feeble;
            const unsigned up = strong;

            for(size_t j=t.size();j>0;--j)
            {
                const segment       &s    = t[j];
                const unit_t         y    = s.y;
                const unit_t         xmin = s.xmin;
                pixrow<uint8_t>     &E_y  = E(y);

                for(unit_t x=s.xmax;x>=xmin;--x)
                {
                    uint8_t       &E0ref = E_y(x);
                    const unsigned E0    = E0ref;
                    if(E0>=up)
                    {
                        E0ref = 255;
                    }
                    else
                    {
                        if(E0>=lo)
                        {
                            E0ref = 127;
                        }
                        else
                        {
                            E0ref = 0;
                        }
                    }
                }
            }

        }

        void gradient:: profile(const tile &t, void *args, lockable &) throw()
        {
            assert(args);
            static_cast<gradient *>(args)->profile(t);
        }
        
    }
    
}


