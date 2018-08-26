//! \file
#ifndef Y_INK_OPS_FILTER_INCLUDED
#define Y_INK_OPS_FILTER_INCLUDED 1

#include "y/ink/pixmap.hpp"

namespace upsylon
{
    namespace ink
    {

        class filter
        {
        public:
            template <typename T,typename U,typename FUNC> static inline
            void apply(pixmap<T>       &target,
                       const pixmap<U> &source,
                       FUNC            &func,
                       engine          &E)
            {
                parameters<T,U,FUNC> proxy = { &target, &source, &func };
                E.run(proxy);
            }

        private:
            template <typename T,typename U,typename FUNC>
            struct parameters
            {
                pixmap<T>       *_target;
                const pixmap<U> *_source;
                FUNC            *_func;

                inline void operator()(const area &zone, lockable &)
                {
                    assert(_target);
                    assert(_source);
                    assert(_func);
                    pixmap<T>       &target = * _target;
                    const pixmap<U> &source = * _source;
                    FUNC            &func   = * _func;

                    const unit_t ymin = zone.lower.y;
                    const unit_t xmin = zone.lower.x;
                    const unit_t xmax = zone.upper.x;
                    for(unit_t y=zone.upper.y;y>=ymin;--y)
                    {
                        typename       pixmap<T>::row &tgt = target[y];
                        const typename pixmap<U>::row &src = source[y];
                        for(unit_t x=xmax;x>=xmin;--x)
                        {
                            tgt[x] = func(src[x]);
                        }
                    }

                }
            };
        };
    }
}

#endif

