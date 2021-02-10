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
                    Apply           &self   = *static_cast<Apply *>(data);
                    Pixmap<T>       &target = self.target;
                    const Pixmap<U> &source = self.source;
                    const Tile      &tile   = worker.tile;
                    FUNC            &func   = self.func;
                    for(size_t t=tile.size();t>0;--t)
                    {
                        const HScan                   &s   = tile[t];
                        Point                          p   = s.begin;
                        const typename Pixmap<U>::Row &src = source[p.y];
                        typename Pixmap<T>::Row       &tgt = target[p.y];
                        for(unit_t i=s.width;i>0;--i,++p.x)
                        {
                            tgt[p.x] = func(src[p.x]);
                        }
                    }
                }
            
            };
        }
        
    }
}

#endif
