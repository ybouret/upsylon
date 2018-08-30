//! \file
#ifndef Y_INK_CHANNELS_INCLUDED
#define Y_INK_CHANNELS_INCLUDED 1

#include "y/ink/pixmaps.hpp"

namespace upsylon
{
    namespace Ink
    {

        struct Channels
        {

            template <typename SCALAR,typename COLOR> static inline
            void Split(Pixmaps<SCALAR>     &target,
                       const Pixmap<COLOR> &source,
                       Engine              &E)
            {
                struct __split<SCALAR,COLOR> proxy = { &target, &source };
                E.run(proxy);
            };


            template <typename SCALAR,typename COLOR>
            struct __split
            {
                Pixmaps<SCALAR>     * _target;
                const Pixmap<COLOR> * _source;

                inline void operator()( const Area &zone, lockable & )
                {
                    assert(_target); assert(_source);
                    Pixmaps<SCALAR>     &target = * _target;
                    const Pixmap<COLOR> &source = *_source;
                    const size_t nch = target.size();
                    Y_INK_AREA_LIMITS(zone);
                    
                    for(unit_t y=ymax;y>=ymin;--y)
                    {
                        const typename Pixmap<COLOR>::row &src = source[y];
                        for(unit_t x=xmax;x>=xmin;--x)
                        {
                            const SCALAR *p = (const SCALAR *) & src[x];
                            for(size_t i=0;i<nch;++i)
                            {
                                target[i][y][x] = *(p++);
                            }
                        }
                    }
                }
            };

        };

    }
}

#endif

