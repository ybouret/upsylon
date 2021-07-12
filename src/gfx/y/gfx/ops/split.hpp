//! \file

#ifndef Y_GFX_OPS_SPLIT_INCLUDED
#define Y_GFX_OPS_SPLIT_INCLUDED 1

#include "y/gfx/stack.hpp"

namespace upsylon
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! split source into individual component
        //
        //______________________________________________________________________
        template <
        typename T,
        typename PIXEL> inline
        void split(stack<T>            &target,
                   const pixmap<PIXEL> &source,
                   broker              &apply,
                   const unsigned      *channels,
                   const size_t         numchans)
        {
            assert(!(NULL==channels&&numchans>0));
            assert(sizeof(PIXEL)>=sizeof(T));
            assert(numchans<=target.size());
            
            struct ops
            {
                stack<T>            &target;
                const pixmap<PIXEL> &source;
                const unsigned      *channels;
                const size_t         numchans;

                static inline void run(const tile &t,
                                       void       *args,
                                       lockable   &) throw()
                {
                    typedef  typename pixmap<PIXEL>::row row_type;
                    ops                  &self     = *static_cast<ops *>(args);
                    stack<T>             &target   = self.target;
                    const pixmap<PIXEL>  &source   = self.source;
                    const unsigned       *channels = self.channels;
                    const size_t          numchans = self.numchans;

                    for(size_t j=t.size();j>0;--j)
                    {
                        const segment   &s    = t[j];
                        const unit_t     y    = s.y;
                        const unit_t     xmin = s.xmin;
                        const row_type  &src  = source[y];
                        for(unit_t x=s.xmax;x>=xmin;--x)
                        {
                            const PIXEL &px = src[x];
                            const T     *p  = (const T *)&px; (void)p;
                            for(size_t i=0;i<numchans;++i)
                            {
                                const unsigned ch = channels[i];
                                target[i](y)(x)   = p[ch];
                            }
                        }
                    }
                }
            };

            ops todo = { target, source, channels, numchans };
            apply( ops::run, &todo );
        }

    }
}

#endif

