#include "y/graphic/ops/gradient.hpp"

namespace upsylon {

    namespace  Graphic {


        Gradient:: ~Gradient() throw()
        {
        }

        Gradient:: Gradient(const size_t W, const size_t H) :
        Pixmap<float>(W,H),
        field(W,H)
        {
        }

        
        void Gradient:: compute( Tiles &tiles, bool doNormalize)
        {
            tiles.cacheAcquireFor<float>();

            struct Task
            {
                Pixmap<float>        *target_;
                const Pixmap<float>  *source_;
                Tiles                 *tiles_;

                static inline void gradientSquared( void *args, parallel &ctx, lockable & ) throw()
                {
                    assert(args);
                    Task                 &task   = *static_cast<Task *>(args);
                    Tile                 &tile   = (*task.tiles_)[ctx.rank];
                    Pixmap<float>        &target = *task.target_;
                    const Pixmap<float>  &source = *task.source_;
                    const Point           lower  = tile.lower;
                    const Point           upper  = tile.upper;

                    float &gmax = tile.get<float>();
                    gmax = 0;
                    for(unit_t y=upper.y,ym1=y-1,yp1=y+1;y>=lower.y;--y,--ym1,--yp1)
                    {
                        const unit_t ym = target->zfh(ym1);
                        const unit_t yp = target->zfh(yp1);
                        Pixmap<float>::RowType       &tgt = target[y];
                        const Pixmap<float>::RowType &src = source[y];

                        for(unit_t x=upper.x,xp1=x+1,xm1=x-1;x>=lower.x;--x,--xm1,--xp1)
                        {
                            const unit_t xm = target->zfw(xm1);
                            const unit_t xp = target->zfw(xp1);
                            float  gx = src[xp]       - src[xm];
                            float  gy = source[yp][x] - source[ym][x];
                            gx *= gx;
                            gy *= gy;
                            const float g = gx+gy;
                            tgt[x] = g;
                            if(g>gmax) gmax=g;
                        }
                    }

                }
            };

            Task task = { this, &field, &tiles };
            tiles.loop().run( task.gradientSquared, &task );
            if(doNormalize)
            {
                normalize(tiles);
            }

        }

        namespace {

            struct NormTask
            {
                float factor;

                float operator()( const float f ) const throw()
                {
                    return f*factor;
                }

            };
        }

        void Gradient:: normalize(Tiles &tiles) throw()
        {
            // reduce gmax
            const size_t nt   = tiles.count;
            float        gmax = tiles[0].get<float>();

            // scan other tiles
            for(size_t i=1;i<nt;++i)
            {
                const float  tmp  = tiles[i].get<float>();
                if(tmp>gmax) gmax = tmp;
            }

            // check
            if(gmax>0)
            {
                const NormTask task = { 1.0f/gmax };
                Pixmap<float> &self = *this;
                Ops::Run(tiles,self,self,task);
            }
        }


    }

}

