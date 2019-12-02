//! \file

#ifndef Y_GRAPHIC_GRADIENT_INCLUDED
#define Y_GRAPHIC_GRADIENT_INCLUDED 1

#include "y/graphic/parallel/ops.hpp"

namespace upsylon {

    namespace  Graphic {

        class Gradient : public Pixmap<float>
        {
        public:
            template <typename T>
            static float ToFloat(const T &) throw();

            explicit Gradient(const size_t W, const size_t H);
            virtual ~Gradient() throw();

            template <typename SOURCE, typename PROC>
            inline void Compute(const SOURCE &source,
                                PROC         &proc,
                                Tiles        &tiles)
            {
                tiles.localAcquire( sizeof(float) );

                struct Task
                {
                    Pixmap<float> *target_;
                    const SOURCE  *source_;
                    PROC          *proc_;
                    Tiles         *tiles_;

                    static inline void gradientSquared( void *args, parallel &ctx, lockable & ) throw()
                    {
                        assert(args);
                        Task          &task   = *static_cast<Task *>(args);
                        Tile          &tile   = (*task.tiles_)[ctx.rank];
                        Pixmap<float> &target = *task.target_;
                        const SOURCE  &source = *task.source_;
                        PROC          &proc   = *task.proc_;
                        const Point    lower  = tile.lower;
                        const Point    upper  = tile.upper;

                        float &gmax = tile.as<float>();
                        gmax = 0;
                        for(unit_t y=upper.y,ym1=y-1,yp1=y+1;y>=lower.y;--y,--ym1,--yp1)
                        {
                            const unit_t ym = target->zfh(ym1);
                            const unit_t yp = target->zfh(yp1);
                            Pixmap<float>::RowType         &tgt = target[y];
                            const typename SOURCE::RowType &src = source[y];

                            for(unit_t x=upper.x,xp1=x+1,xm1=x-1;x>=lower.x;--x,--xm1,--xp1)
                            {
                                const unit_t xm = target->zfw(xm1);
                                const unit_t xp = target->zfw(xp1);
                                float  gx = proc(src[xp])       - proc(src[xm]);
                                float  gy = proc(source[yp][x]) - proc(source[ym][x]);
                                gx *= gx;
                                gy *= gy;
                                const float g = gx+gy;
                                tgt[x] = g;
                                if(g>gmax) gmax=g;
                            }
                        }

                    }
                };

                Task task = { this, &source, &proc,  &tiles };
                tiles.loop().run( task.gradientSquared, &task );
                normalize(tiles);

            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Gradient);
            void normalize(Tiles &tiles) throw();

        };

    }
}

#endif
