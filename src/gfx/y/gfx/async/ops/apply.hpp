//! \file

#ifndef Y_GFX_ASYNC_APPLY_INCLUDED
#define Y_GFX_ASYNC_APPLY_INCLUDED 1

#include "y/gfx/async/broker.hpp"

namespace upsylon
{
    namespace GFX
    {
        template <typename T> class Pixmap;
        
        namespace Async
        {
            template <typename T, typename U, typename FUNC>
            struct Apply
            {
                Pixmap<T>       &target;
                const Pixmap<U> &source;
                FUNC            &func;
                
                static inline void run(Async::Worker &worker,
                                       lockable      &,
                                       void          *data)
                {
                    assert(data);
                    Apply      &self = *static_cast<Apply *>(data);
                    const Tile &tile = worker.tile;
                    for(size_t t=tile.size();t>0;--t)
                    {
                        const HScan &hs = tile[t];
                        Point        p  = hs.begin;
                        const typename Pixmap<U>::Row &src = self.source[p.y];
                        typename Pixmap<T>::Row       &tgt = self.target[p.y];
                        for(unit_t i=hs.width;i>0;--i,++p.x)
                        {
                            tgt[p.x] = self.func(src[p.x]);
                        }
                    }
                }
            
            };
        }
        
    }
}

#endif
