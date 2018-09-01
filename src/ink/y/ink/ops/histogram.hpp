//! \file
#ifndef Y_INK_HIST_INCLUDED
#define Y_INK_HIST_INCLUDED

#include "y/ink/pixmaps.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! Histogram manipulation
        class Histogram : public Object
        {
        public:
            static const size_t BINS = 256; //!< 8-bit version

            size_t bins[BINS]; //!< bins with their count

            explicit Histogram() throw(); //!< set bins to 0
            virtual ~Histogram() throw(); //!< destructor

            void reset() throw(); //!< set bins to 0
            void append( const size_t *other_bins ) throw(); //!< low level append

            //! build histogram with projection function
            template <typename T,typename FUNC>
            inline void build( const Pixmap<T> &source, FUNC &type2byte, Engine &E, const bool init=true )
            {
                E.acquire_all( sizeof(bins) );
                if(init)
                {
                    reset();
                }
                __build<T,FUNC> proxy = { &source, &type2byte };
                E.run(proxy);
                const Tiles &zones = E.tiles;
                for(size_t i=zones.size();i>0;--i)
                {
                    const size_t *b = &(zones[i]->cache.get<size_t>());
                    append(b);
                }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Histogram);
            template <typename T,typename FUNC>
            struct __build
            {
                const Pixmap<T> * _source;
                FUNC            * _func;

                inline void operator()( const Tile &tile, lockable & )
                {
                    assert(_source);
                    assert(_func);
                    const Pixmap<T> &source = *_source;
                    FUNC            &func   = *_func;
                    size_t          *b      = & tile.cache.get<size_t>();
                    Y_INK_AREA_LIMITS(tile);
                    for(unit_t y=ymax;y>=ymin;--y)
                    {
                        const typename Pixmap<T>::Row &src = source[y];
                        for(unit_t x=xmax;x>=xmin;--x)
                        {
                            ++b[ uint8_t(func( src[x] )) ];
                        }
                    }
                }
            };
        };
    }
}

#endif

