
#include "y/gfx/edges/keep-max.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace edges
        {

            keep_max:: keep_max(const unit_t W, const unit_t H) :
            histogram(),
            pixmap<uint8_t>(W,H)
            {
            }

            keep_max:: ~keep_max() throw()
            {
            }

            void keep_max:: operator()(broker &apply, gradient &G)
            {
                // prepare memory
                local_caches &caches = apply.caches;
                caches.make<size_t>( bins );
                reset();

                struct ops
                {
                    keep_max &kmax;
                    gradient &grad;

                    static inline
                    void run(const tile &t, void *args, lockable &)
                    {
                        assert( t.cache->is<size_t>() );
                        assert( t.cache->tell() >= bins);
                        assert( args );
                        typedef gradient::vertex vtx;
                        size_t            *hist  = & (t.cache->as<size_t>());
                        ops               &self  = *static_cast<ops *>(args);
                        keep_max          &kmax  = self.kmax;
                        pixmap<uint8_t>   &bmax  = kmax;
                        gradient          &grad  = self.grad;
                        pixmap<vtx>       &probe = grad.probe;

                        for(size_t j=t.size();j>0;--j)
                        {
                            const segment       &s       = t[j];
                            const unit_t         y       = s.y;
                            const unit_t         xmin    = s.xmin;
                            pixrow<uint8_t>     &bmax_y  = bmax(y);
                            const pixrow<float> &grad_y  = grad(y);
                            pixrow<vtx>         &probe_y = probe(y);

                            for(unit_t x=s.xmax;x>=xmin;--x)
                            {
                                const float G0 = grad_y(x);
                                const vtx   d  = probe_y(x);
                                const float Gm = grad[y-d.y][x-d.x];
                                const float Gp = grad[y+d.y][x+d.x];
                                if(G0<Gm||G0<Gp)
                                {
                                    bmax_y(x) = 0;
                                    probe_y(x) = vtx(0,0);
                                }
                                else
                                {
                                    const uint8_t b0 = crux::convert::to_byte(G0);
                                    bmax_y(x) = b0;
                                    ++hist[b0];
                                }
                            }
                        }

                    }

                };

                ops todo = { *this, G };
                apply( ops::run, &todo );

                // merge memory
                for(size_t i=0;i<caches.size();++i)
                {
                    add( &(caches[i]->as<size_t>()) );
                }

            }


        }

    }

}


