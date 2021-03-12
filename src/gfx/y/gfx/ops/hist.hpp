

//! \file

#ifndef Y_GFX_OPS_HIST_INCLUDED
#define Y_GFX_OPS_HIST_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/color/histogram.hpp"

namespace upsylon
{
    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! computing histograms
        //
        //______________________________________________________________________
        struct compute_histogram
        {
            //! add pixmap to historgram
            /**
             \param H an historgram
             \param pxm a pixmap
             \param apply a broker for pxm
             \param conv  convert<uint8_t,T>::from, for example
             */
            template <typename T, typename FUNC> static inline
            void update(histogram       &H,
                        const pixmap<T> &pxm,
                        broker          &apply,
                        FUNC            &conv)
            {
                //--------------------------------------------------------------
                //
                // check memory for each tile
                //
                //--------------------------------------------------------------
                local_caches &caches = apply.tess.caches;
                caches.make<size_t>( histogram::bins );

                //--------------------------------------------------------------
                //
                // local ops
                //
                //--------------------------------------------------------------
                {
                    struct ops
                    {
                        const pixmap<T>   &pxm;
                        FUNC              &conv;
                        static inline void run(const tile &t,
                                               void       *args,
                                               lockable   &) throw()
                        {
                            assert(t.cache->is<size_t>());
                            assert(t.cache->tell()>=histogram::bins);
                            ops             &self  = *static_cast<ops *>(args);
                            const pixmap<T> &pxm   = self.pxm;
                            size_t          *hist  = & t.cache->as<size_t>();
                            FUNC            &conv  = self.conv;
                            for(size_t j=t.size();j>0;--j)
                            {
                                const segment   &s    = t[j];
                                const pixrow<T> &r    = pxm(s.y);
                                const unit_t     xmin = s.xmin;
                                for(unit_t x=s.xmax;x>=xmin;--x)
                                {
                                    const uint8_t indx = uint8_t( conv(r(x)) );
                                    ++hist[indx];
                                }
                            }
                        }
                    };

                    // run local
                    ops todo = { pxm, conv };
                    apply(ops::run,&todo);
                }

                //--------------------------------------------------------------
                //
                // reduction
                //
                //--------------------------------------------------------------
                const size_t nc = caches.size();
                for(size_t i=0;i<nc;++i)
                {
                    const local_memory &mem = *caches[i];
                    const size_t *h = & mem.as<size_t>();
                    H.add(h);
                }

            }

        };

    }

}

#endif

