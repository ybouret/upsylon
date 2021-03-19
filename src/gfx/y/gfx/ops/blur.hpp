//! \file

#ifndef Y_GFX_BLUR_INCLUDED
#define Y_GFX_BLUR_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/pblock.hpp"
#include "y/gfx/color/convert.hpp"

namespace upsylon
{
    namespace graphic
    {
        

        class blur
        {
        public:
            static const float expand; //!< 2*sqrt(2*log(2))


            static unit_t r_max_for(const float sig) throw();

            explicit blur(const float sig);
            virtual ~blur() throw();


            const unit_t          r_max;  //!< rmax_for(sigma)
            const pixmap<float>   weight; //! (1+rmax,1+rmax)
            const float           factor; //!< normalize factor
            const float           sigma;  //!< std dev
            mutable pblock<float> data;   //!< workspace

            float direct_sum()     const throw();
            float sorted_sum()     const throw();
            float direct_average() const throw();
            float sorted_average() const throw();



            template <typename PIXEL, typename T, const size_t NCH>
            PIXEL compute_at(const coord          center,
                             const pixmap<PIXEL> &source,
                             PIXEL               *pstack) const throw()
            {
                assert(sizeof(PIXEL)<=sizeof(T)*NCH);

                // populate pstack
                const unit_t r = r_max;
                {
                    size_t       k = 0;
                    for(unit_t   y=-r;y<=r;++y)
                    {
                        const pixrow<PIXEL> &source_y = source[center.y+y];
                        for(unit_t x=-r;x<=r;++x)
                        {
                            pstack[k++] = source_y[x+center.x];
                        }
                    }
                    assert(data.size==k);
                }

                // prepare result
                PIXEL result(0);
                T    *target = (T *)&result;

                // compute per channel
                for(size_t ch=0;ch<NCH;++ch)
                {
                    size_t k=0;
                    for(unit_t   y=-r;y<=r;++y)
                    {
                        const pixrow<float> &W_y = weight( (y<0?-y:y) );
                        for(unit_t x=-r;x<=r;++x)
                        {
                            const float  w = W_y(x<0?-x:x);
                            const PIXEL &p = pstack[k];
                            const T     &v = *(((const T *)&p)+ch);
                            data[k] = float(v) * w;
                            ++k;
                        }
                    }
                    assert(k==data.size);
                    const float f = direct_average();
                    target[ch] = convert<T,float>::from(f);
                }
                return result;

            }

            template <
            typename     PIXEL,
            typename     T,
            const size_t NCH>
            void compute(const tile          &t,
                         pixmap<PIXEL>       &target,
                         const pixmap<PIXEL> &source) const throw()
            {
                local_cache &cache = t.cache;
                assert(cache->is<PIXEL>());
                assert(cache->tell()>=data.size);
                PIXEL *pstack = & (cache->as<PIXEL>());

                for(size_t j=t.size();j>0;--j)
                {
                    const segment &s    = t[j];
                    const unit_t   y    = s.y;
                    const unit_t   xmin = s.xmin;
                    for(unit_t x=s.xmax;x>=xmin;--x)
                    {
                        const coord center(x,y);
                        target(center) = compute_at<PIXEL,T,NCH>(center,source,pstack);
                    }
                }
            }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(blur);
        };

    }



}

#endif

