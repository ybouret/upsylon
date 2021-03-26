
//! \file

#ifndef Y_GFX_GAUSSIAN_BLUR_INCLUDED
#define Y_GFX_GAUSSIAN_BLUR_INCLUDED 1

#include "y/gfx/area/patch.hpp"
#include "y/gfx/color/rgb.hpp"
#include "y/gfx/pblock.hpp"
#include "y/mkl/types.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace crux
        {
            //! convert to closest type
            template <typename T,typename U>
            struct to_closest
            {
                static T from(const U) throw(); //!< prototype
            };

            //! convert identity
            template <typename T>
            struct to_closest<T,T>
            {
                //! prototype
                static inline T from(const T x) throw() { return x; }
            };

            //! convert [0..255.0f]->uint8_t
            template <>
            struct to_closest<uint8_t,float>
            {
                //! prototype
                static inline uint8_t from(const float x) throw() { return uint8_t( floorf(x+0.5f) ); }
            };

            //! convert [0..255]->uint8_t
            template <>
            struct to_closest<uint8_t,double>
            {
                //! prototype
                static inline uint8_t from(const float x) throw() { return uint8_t( floor(x+0.5) ); }
            };


        }

        //______________________________________________________________________
        //
        //
        //! gaussian blur
        //
        //______________________________________________________________________
        template <typename T>
        class gaussian_blur : public patch<T>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            Y_DECL_ARGS(T,type);           //!< aliasesd
            typedef patch<T> patch_type;   //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            inline virtual ~gaussian_blur() throw() {}

            //! setup with automatic symmetrical layout
            inline explicit gaussian_blur(const unit_t W,
                                          const unit_t H,
                                          param_type   sigma_x,
                                          param_type   sigma_y=0) :
            patch_type(W,H),
            factor(1),
            weight(0)
            {
                setup(sigma_x,sigma_y);
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const_type factor; //!< 1/weight
            const_type weight; //!< total weight

            //__________________________________________________________________
            //
            //! compute using tile broker
            //__________________________________________________________________
            template <typename PIXEL, typename U, const unsigned NCH>
            void compute(pixmap<PIXEL>       &target,
                         broker              &apply,
                         const pixmap<PIXEL> &source) const
            {

                struct ops
                {
                    const gaussian_blur &g_blur;
                    pixmap<PIXEL>       &target;
                    const pixmap<PIXEL> &source;

                    static inline void run(const tile &t, void *args, lockable &) throw()
                    {
                        assert(args);
                        ops &_ = *static_cast<ops *>(args);
                        _.g_blur.template computeN<PIXEL,U,NCH>(t,_.target,_.source);
                    }
                };

                ops todo = { *this, target, source };
                apply(ops::run,&todo);
            }

            //! self type cover
            inline
            void cover(pixmap<T> &target, broker &apply, const pixmap<T> &source)
            {
                compute<T,T,1>(target,apply,source);
            }

            //! rgb cover
            inline
            void cover(pixmap<rgb> &target, broker &apply, const pixmap<rgb> &source)
            {
                compute<rgb,uint8_t,3>(target,apply,source);
            }

            //! rgb(a) cover
            inline
            void cover(pixmap<rgba> &target, broker &apply, const pixmap<rgba> &source)
            {
                compute<rgba,uint8_t,3>(target,apply,source);
            }

            //! uint8_t cover
            inline
            void cover(pixmap<uint8_t> &target, broker &apply, const pixmap<uint8_t> &source)
            {
                compute<uint8_t,uint8_t,1>(target,apply,source);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(gaussian_blur);

            inline void setup(param_type sigma_x, mutable_type sigma_y)
            {
                static const_type one(1);
                assert(sigma_x>0);
                if(sigma_y<=0) sigma_y = sigma_x;
                //std::cerr << "sx: " << sigma_x << ", sy:" << sigma_y << std::endl;

                const_type dx2 = twice( square_of(sigma_x) );
                const_type dy2 = twice( square_of(sigma_y) );
                for(unit_t y=this->lower.y;y<=this->upper.y;++y)
                {
                    patch_row<T> &g   = (*this)[y];
                    const_type    Y2  = const_type(y*y)/dy2;
                    for(unit_t x=this->lower.x;x<=this->upper.x;++x)
                    {
                        const_type  X2    = const_type(x*x)/dx2;
                        aliasing::_(g[x]) = mkl::exp_of( -(X2+Y2) );
                    }
                }
                pblock<mutable_type> tmp(this->items);
                aliasing::_(weight) = this->sum(&tmp[0]);
                aliasing::_(factor) = one/weight;
                //std::cerr << "weight=" << weight << std::endl;
            }


            template <typename PIXEL, typename U, const unsigned NCH> inline
            void compute1(PIXEL               &target,
                         const pixmap<PIXEL> &source,
                         const coord          pos) const throw()
            {
                mutable_type   acc[NCH];
                this->template ld<PIXEL,U,NCH>(acc,source,pos);
                U *q = (U *) &target;
                for(unsigned ch=0;ch<NCH;++ch)
                {
                    const_type value = acc[ch]*this->factor;
                    q[ch] = crux::to_closest<U,T>::from(value); // TODO: improve
                }
            }


            template <typename PIXEL, typename U, const unsigned NCH> inline
            void computeN(const tile          &t,
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
                        compute1<PIXEL,U,NCH>(target(center),source,center);
                    }
                }
            }


        };

    }

}

#endif

