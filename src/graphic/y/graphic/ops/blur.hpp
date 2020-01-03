
//! \file

#ifndef Y_GRAPHIC_BLUR_INCLUDED
#define Y_GRAPHIC_BLUR_INCLUDED 1

#include "y/graphic/pixmap.hpp"
#include "y/graphic/convert.hpp"
#include "y/graphic/parallel/tiles.hpp"

namespace upsylon {

    namespace Graphic  {

        //! base class for Blur_
        class Blur_
        {
        public:
            //! compute size for a given parameter
            static size_t getSizeFor( const float sigma ) throw();

            virtual ~Blur_() throw(); //!< cleanup

            const float  sigma;      //!< blur parameter
            const float  sig2;       //!< sigma*sigma
            const float  scale;      //!< 1.0f/(2*sigma2)
            const size_t delta;      //!< size
            const unit_t upperRange; //!< delta-1
            const unit_t lowerRange; //!< -upperRange

        protected:
            //! compute all parameters
            explicit Blur_( const float sig );

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blur_);
        };

        //! holding blur components
        typedef Pixmap<float> BlurMap;

        //! apply blur to a pixmap
        class Blur : public Blur_, public BlurMap
        {
        public:

            explicit Blur(const float sig); //!< setup
            virtual ~Blur() throw();        //!< cleanup

            //! apply blur in parallel
            template <typename TYPE,
            typename           T,
            const size_t       DIM>
            inline void apply_(Pixmap<TYPE>        &target,
                               const Pixmap<TYPE>  &source,
                               Tiles               &tiles) const
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

            template <typename T>
            void apply( Pixmap<T> &, const Pixmap<T> &, Tiles &) const;


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
                            const BlurMap::RowType               &By  = (*this)[ abs_of(dy) ];
                            const typename Pixmap<TYPE>::RowType &Sy = source(y+dy);
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

                        T    *t   = (T *)&target_y[x];;
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
