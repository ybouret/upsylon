//! \file

#ifndef Y_GFX_BLUR_INCLUDED
#define Y_GFX_BLUR_INCLUDED 1

#include "y/gfx/pixmaps.hpp"
#include "y/gfx/pblock.hpp"

namespace upsylon
{
    namespace graphic
    {
        namespace crux
        {
            //! converting float to a type
            template <typename T>
            struct float_to
            {
                static T from(const float) throw(); //!< prototype
            };
            
            //! float to float...
            template <>
            struct float_to<float>
            {
                static float from(const float x) throw() { return x; } //!< ID
            };
            
            //! uint8_t to float
            template <>
            struct float_to<uint8_t>
            {
                static float from(const uint8_t x) throw()
                {
                    return uint8_t( floorf(x+0.5f) );
                } //!< nearest
            };
        }
        
        //______________________________________________________________________
        //
        //
        //! blur computation
        //
        //______________________________________________________________________
        class blur : public entity
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            typedef unsigned_int<sizeof(float)>::type count_t; //!< alias
            
            //! local weigth
            struct weight_t
            {
                count_t count; //!< number of count
                float   value; //!< effective value
            };
            
            static const float expand; //!< 2*sqrt(2*log(2))
            
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit blur(const float sig); //!< setup
            virtual ~blur() throw();        //!< cleanup
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unit_t           delta;  //!< exp( -(delta^2+delta^2)/(2*sigma^2) ) <= 1/256
            const pblock<weight_t> weight; //!< 0->(delta^2+delta^2) => 1+2*delta^2 value
            const float            factor; //!< normalization factor
            const size_t           counts; //!< number of different r^2
            const size_t           length; //!< 1+2*delta
            const float            sigma;  //!< keep track
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static unit_t delta_for(const float sig) throw(); //!< delta for a given sigma
            
            //__________________________________________________________________
            //
            //! single point direct computation
            //__________________________________________________________________
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
                // average by channels
                //
                //--------------------------------------------------------------
                PIXEL result(0);
                {
                    T          *target = (T *)&result;
                    const float tmp    = factor;
                    for(size_t ch=0;ch<NCH;++ch)
                    {
                        target[ch] = crux::float_to<T>::from(acc[ch]*tmp);
                    }
                }
                
                return result;
            }
            
            //__________________________________________________________________
            //
            //! compute for one tile
            //__________________________________________________________________
            template <
            typename     PIXEL,
            typename     T,
            const size_t NCH>
            void compute1(const tile          &t,
                          pixmap<PIXEL>       &target,
                          const pixmap<PIXEL> &source) const throw()
            {
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
            
            //__________________________________________________________________
            //
            //! compute using tile broker
            //__________________________________________________________________
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
            
            //! implemented for
            /**
             - float, uint8_t, rgb
             - rgba, mix colors only
             */
            template <typename T>
            void cover(pixmap<T> &, broker &, const pixmap<T> &) const;
            
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(blur);
            
        };
        
    }
    
    
    
}

#endif

