#include "y/gfx/edges/gradient.hpp"
#include "y/gfx/color/histogram.hpp"


namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {

            gradient:: gradient(const unit_t W, const unit_t H) :
            pixmap<float>(W,H),
            probe(W,H)
            {
            }

            gradient:: ~gradient() throw()
            {
            }


            float gradient:: compute(const pixmap<float>  &field,
                                     broker               &apply,
                                     const shared_filters &delta)
            {

                //--------------------------------------------------------------
                // setup with enough memory for histogram
                //--------------------------------------------------------------

                static const size_t hbytes = histogram::bins * sizeof(size_t);
                static const size_t floats = hbytes/sizeof(float);
                assert( has_same_metrics_than(field) );
                assert( has_same_metrics_than(apply) );

                local_caches &caches = apply.caches;
                caches.make<float>( floats );

                //--------------------------------------------------------------
                // parallel run
                //--------------------------------------------------------------
                struct ops
                {
                    pixmap<float>       &fgrad;
                    pixmap<vertex>      &probe;
                    const pixmap<float> &field;
                    const filter_type   &del_x;
                    const filter_type   &del_y;

                    static inline
                    void run(const tile &t,
                             void       *args,
                             lockable   &) throw()
                    {
                        ops                 &self  = *static_cast<ops *>(args);
                        const pixmap<float> &field = self.field;
                        const filter_type   &del_x = self.del_x;
                        const filter_type   &del_y = self.del_y;
                        pixmap<float>       &fgrad = self.fgrad;
                        pixmap<vertex>      &probe = self.probe;
                        float                lmax  = 0;

                        for(size_t j=t.size();j>0;--j)
                        {
                            const segment  &s       = t[j];
                            const unit_t    y       = s.y;
                            const unit_t    xmin    = s.xmin;
                            pixrow<float>  &fgrad_y = fgrad(y);
                            pixrow<vertex> &probe_y = probe(y);

                            for(unit_t x=s.xmax;x>=xmin;--x)
                            {
                                const coord p(x,y);
                                float       gx = 0;
                                float       gy = 0;
                                del_x.put(gx,field,p);
                                del_y.put(gy,field,p);
                                const float g2 = gx*gx + gy*gy;
                                if(g2>0.0f)
                                {
                                    const float gg   = sqrtf(g2);
                                    if(gg>lmax) lmax = gg;
                                    (gx/=gg)  += 0.5f;
                                    (gy/=gg)  += 0.5f;
                                    fgrad_y(x) = gg;
                                    probe_y(x) = vertex( int8_t(floor(gx)), int8_t(floor(gy)) );
                                    assert( probe_y(x).norm1() > 0 );
                                }
                                else
                                {
                                    fgrad_y(x) = 0.0f;
                                    probe_y(x) = vertex(0,0);
                                }
                            }
                        }

                        t.cache->as<float>() = lmax;
                    }
                };

                ops todo = { *this, probe, field, *(delta->x), *(delta->y) };
                apply(ops::run,&todo);

                //--------------------------------------------------------------
                // reduction for max
                //--------------------------------------------------------------
                const float gmax = caches.get_max<float>();

                //--------------------------------------------------------------
                // normalize
                //--------------------------------------------------------------
                if(gmax>0)
                {
                    caches.set(gmax);
                    struct __
                    {
                        static inline void normalize(const tile &t, void *args, lockable &) throw()
                        {
                            const float gfac = 1.0f / t.cache->as<float>();
                            gradient   &self = *static_cast<gradient *>(args);
                            for(size_t j=t.size();j>0;--j)
                            {
                                const segment  &s      = t[j];
                                const unit_t    y      = s.y;
                                const unit_t    xmin   = s.xmin;
                                pixrow<float>  &self_y = self(y);

                                for(unit_t x=s.xmax;x>=xmin;--x)
                                {
                                    self_y(x) *= gfac;
                                }
                            }
                        }
                    };
                    apply(__::normalize, static_cast<gradient *>(this) );
                }

                return gmax;
            }

        }

    }

}
