
//! \file

#ifndef Y_GRAPHIC_BLUR_INCLUDED
#define Y_GRAPHIC_BLUR_INCLUDED 1

#include "y/graphic/pixmap.hpp"
#include "y/graphic/parallel/tiles.hpp"
#include "y/graphic/convert.hpp"

namespace upsylon {

    namespace Graphic  {

        class Blur_
        {
        public:
            static size_t getSizeFor( const float sigma ) throw();

            virtual ~Blur_() throw();

            const float  sigma;
            const float  sig2;
            const float  scale;
            const size_t delta;
            const unit_t upperRange;
            const unit_t lowerRange;

        protected:
            explicit Blur_( const float sig );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blur_);
        };

        typedef Pixmap<float> BlurMap;

        class Blur : public Blur_, public BlurMap
        {
        public:
            explicit Blur(const float sig);
            virtual ~Blur() throw();


            template <typename TYPE,
            typename           T,
            const size_t       DIM>
            inline void apply(Pixmap<TYPE>        &target,
                              const Pixmap<TYPE>  &source,
                              Tiles               &tiles)
            {
                struct Task
                {
                    Pixmap<TYPE>        *target_;
                    const Pixmap<TYPE>  *source_;
                    const Tiles         *tiles_;
                    const Blur          *blur;

                    static inline
                    void Run( void *args, parallel &ctx, lockable & ) throw()
                    {
                        Task       &task  = *static_cast<Task *>(args);
                        const Tile &tile = (*task.tiles_)[ctx.rank];
                        task.blur->template apply<TYPE,T,DIM>( *task.target_, *task.source_, tile );
                    }
                };

                Task task = { &target, &source, &tiles, this };
                tiles.loop().run( Task::Run, &task );
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blur);
            template <typename TYPE,
            typename           T,
            const size_t       DIM>
            inline void apply(Pixmap<TYPE>       &target,
                              const Pixmap<TYPE> &source,
                              const Tile         &tile) const throw()
            {
                const Point  lo  = tile.lower;
                const Point  up  = tile.upper;
                const unit_t dlo = lowerRange;
                const unit_t dhi = upperRange;

                float sum[DIM] = { 0 };
                for(unit_t y=up.y;y>=lo.y;--y)
                {
                    typename Pixmap<TYPE>::RowType &target_y = target[y];
                    for(unit_t x=up.x;x>=lo.x;--x)
                    {
                        memset(sum,0,sizeof(sum));
                        for(unit_t dy=dhi;dy>=dlo;--dy)
                        {
                            const unit_t            yy = y + dy;
                            const BlurMap::RowType &By  = (*this)[ abs_of(dy) ];
                            const typename Pixmap<TYPE>::RowType &Sy = source(yy);
                            for(unit_t dx=dhi;dx>=dlo;--dx)
                            {
                                const TYPE  &src    = Sy(x+dx);
                                const float  weight = By[ abs_of(dx) ];
                                const T     *s      = (const T *)&src;
                                for(size_t dim=0;dim<DIM;++dim)
                                {
                                    sum[dim] += weight * Convert::Get<float,T>( s[dim] );
                                }
                            }
                        }

                        TYPE &tgt = target_y[x];
                        T    *t   = (T *)&tgt;
                        for(size_t dim=0;dim<DIM;++dim)
                        {
                            t[dim] = Convert::Get<T,float>( sum[dim] );
                        }

                    }
                }
            }
        };

    }
}

#endif
