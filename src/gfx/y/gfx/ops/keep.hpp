

//! \file

#ifndef Y_GFX_OPS_KEEP_INCLUDED
#define Y_GFX_OPS_KEEP_INCLUDED 1

#include "y/gfx/pixmap.hpp"

namespace upsylon
{
    namespace graphic
    {

        struct keep
        {

#define Y_GFX_KEEP_ARGS const uint8_t value, const uint8_t threshold

            typedef bool (*compare)(Y_GFX_KEEP_ARGS);

            //! strict background
            static inline bool lt(Y_GFX_KEEP_ARGS) throw()
            {
                return value<threshold;
            }

            //! background
            static inline bool leq(Y_GFX_KEEP_ARGS) throw()
            {
                return value<=threshold;
            }

            //! strict foreground
            static inline bool gt(Y_GFX_KEEP_ARGS) throw()
            {
                return value>threshold;
            }

            //! foreground
            static inline bool geq(Y_GFX_KEEP_ARGS) throw()
            {
                return value>=threshold;
            }


            template <typename T, typename CONV> static inline
            void transfer(pixmap<T>       &target,
                          const pixmap<T> &source,
                          broker          &apply,
                          CONV            &conv,
                          compare          comp,
                          const uint8_t    threshold,
                          const T          discarded) throw()
            {

                struct ops
                {
                    pixmap<T>       &target;
                    const pixmap<T> &source;
                    CONV            &conv;
                    compare          comp;
                    uint8_t          threshold;
                    T                discarded;

                    static inline void run(const tile &t, void *args, lockable &) throw()
                    {
                        assert(args);
                        ops &self = *static_cast<ops *>(args);
                        pixmap<T>       &target    = self.target;
                        const pixmap<T> &source    = self.source;
                        CONV            &conv      = self.conv;
                        compare          comp      = self.comp; assert(comp);
                        const uint8_t    threshold = self.threshold;
                        const T          discarded = self.discarded;

                        for(size_t j=t.size();j>0;--j)
                        {
                            const segment   &s    = t[j];
                            const unit_t     y    = s.y;
                            const unit_t     xmin = s.xmin;
                            const pixrow<T> &src  = source(y);
                            pixrow<T>       &tgt  = target(y);

                            for(unit_t x=s.xmax;x>=xmin;--x)
                            {
                                const T      &pix = src(x);
                                const uint8_t val = conv(pix);
                                if( comp(val,threshold) )
                                {
                                     tgt(x) = pix;
                                }
                                else
                                {
                                    tgt(x) = discarded;
                                }
                            }

                        }

                    }
                };

                ops todo = { target, source, conv, comp, threshold, discarded };
                apply(ops::run,&todo);
            }

        };

    }

}

#endif
