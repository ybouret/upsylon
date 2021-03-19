//! \file

#ifndef Y_GFX_BLUR_INCLUDED
#define Y_GFX_BLUR_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/pblock.hpp"
#include "y/gfx/color/convert.hpp"
#include "y/container/matrix.hpp"

namespace upsylon
{
    namespace graphic
    {
        

        class blur
        {
        public:
            typedef unsigned_int<sizeof(float)>::type count_t;
            struct weight_t
            {
                count_t count;
                float   value;
                inline float operator*() const throw() { return value; }
            };
            
            static const float expand; //!< 2*sqrt(2*log(2))


            static unit_t delta_for(const float sig) throw();

            explicit blur(const float sig);
            virtual ~blur() throw();


            const unit_t           delta;  //!< exp( -(delta^2+delta^2)/(2*sigma^2) ) <= 1/256
            const pblock<weight_t> weight; //!< 0->(delta^2+delta^2) => 1+2*delta^2 value
            const float            factor; //!< normalization factor
            const size_t           counts; //!< number of different r^2
            const size_t           length; //!< 1+2*delta
            const float            sigma;  //!< keep track
            
            template <typename PIXEL, typename T, const size_t NCH>
            PIXEL compute_at(const coord          center,
                             const pixmap<PIXEL> &source) const throw()
            {
                assert(sizeof(PIXEL)<=sizeof(T)*NCH);
                assert(NCH<=4);
                //--------------------------------------------------------------
                //
                // sum by channels
                //
                //--------------------------------------------------------------
                float        acc[4] = {0,0,0,0};
                const unit_t r      = delta;
                {
                    for(unit_t y=-r;y<=r;++y)
                    {
                        const unit_t y2=y*y;
                        const pixrow<PIXEL> &source_y = source[center.y+y];
                        for(unit_t x=-r;x<=r;++x)
                        {
                            const unit_t x2 = x*x;
                            const float  w = weight[x2+y2].value;
                            const PIXEL &p = source_y[x+center.x];
                            const T     *q = (const T*)&p;
                            for(size_t ch=0;ch<NCH;++ch)
                            {
                                acc[ch] += w * float(q[ch]);
                            }
                        }
                    }
                }
                
                //--------------------------------------------------------------
                //
                // avergae by channels
                //
                //--------------------------------------------------------------
                PIXEL result(0);
                {
                    T          *target = (T *)&result;
                    const float tmp    = factor;
                    for(size_t ch=0;ch<NCH;++ch)
                    {
                        target[ch] = T(acc[ch]*tmp);
                    }
                }
                
                return result;
            }
            
            
            template <
            typename     PIXEL,
            typename     T,
            const size_t NCH>
            void compute1(const tile          &t,
                          pixmap<PIXEL>       &target,
                          const pixmap<PIXEL> &source) const throw()
            {
                
                // run on tile
                for(size_t j=t.size();j>0;--j)
                {
                    const segment &s    = t[j];
                    const unit_t   y    = s.y;
                    const unit_t   xmin = s.xmin;
                    for(unit_t x=s.xmax;x>=xmin;--x)
                    {
                        const coord center(x,y);
                        target(center) = compute_at<PIXEL,T,NCH>(center,source);
                    }
                }
            }

            template <typename PIXEL, typename T, const size_t NCH>
            void compute_for(pixmap<PIXEL>       &target,
                             broker              &apply,
                             const pixmap<PIXEL> &source) const
            {
                
                struct ops
                {
                    const blur          &myself;
                    pixmap<PIXEL>       &target;
                    const pixmap<PIXEL> &source;

                    static inline void run(const tile &t, void *args, lockable &) throw()
                    {
                        assert(args);
                        ops &_ = *static_cast<ops *>(args);
                        _.myself.template compute1<PIXEL,T,NCH>(t,_.target,_.source);
                    }
                };

                ops todo = { *this, target, source };
                apply(ops::run,&todo);
            }

            void compute(pixmap<float>   &target, broker &apply,const pixmap<float>   &source) const;
            void compute(pixmap<rgb>     &target, broker &apply,const pixmap<rgb>     &source) const;
            void compute(pixmap<rgba>    &target, broker &apply,const pixmap<rgba>    &source) const;
            void compute(pixmap<uint8_t> &target, broker &apply,const pixmap<uint8_t> &source) const;

            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(blur);

        };

    }



}

#endif

