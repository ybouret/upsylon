//! \file

#ifndef Y_GFX_OPS_3x3_INCLUDED
#define Y_GFX_OPS_3x3_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/pixel.hpp"

namespace upsylon
{
    namespace graphic
    {

        struct _3x3
        {
            template <
            typename T,
            typename U,
            typename FUNC,
            typename CONV>
            static inline void to(pixmap<T>       &target,
                                  const pixmap<U> &source,
                                  broker          &apply,
                                  FUNC            &op,
                                  CONV            &conv
                                  )
            {

                assert( target.has_same_metrics_than(source)     );
                assert( target.has_same_metrics_than(apply.tess) );


                struct ops
                {
                    pixmap<T>       &target;
                    const pixmap<U> &source;
                    FUNC            &op;
                    CONV            &conv;

                    static inline void run(const tile &t,
                                           void       *args,
                                           lockable   &) throw()
                    {
                        assert(args);

                        ops              &self   = *static_cast<ops *>(args);
                        pixmap<T>        &target = self.target;
                        const pixmap<U>  &source = self.source;
                        FUNC             &op     = self.op;
                        CONV             &conv   = self.conv;

                        for(size_t j=t.size();j>0;--j)
                        {
                            const segment &s    = t[j];
                            const unit_t   y    = s.y;
                            const unit_t   xmin = s.xmin;
                            for(unit_t x=s.xmax;x>=xmin;--x)
                            {
                                const coord p(x,y);
                                U arr[9] =
                                {
                                    source(y)(x),
                                    source[p+area::delta[0]],
                                    source[p+area::delta[1]],
                                    source[p+area::delta[2]],
                                    source[p+area::delta[3]],
                                    source[p+area::delta[4]],
                                    source[p+area::delta[5]],
                                    source[p+area::delta[6]],
                                    source[p+area::delta[7]],
                                };
                                const U u    = op(arr); 
                                target(y)(x) = conv(u);
                            }
                        }

                    }
                };

                ops todo = { target, source, op, conv };
                apply( ops::run, &todo );
                

            }
        };

    }

}

#endif

