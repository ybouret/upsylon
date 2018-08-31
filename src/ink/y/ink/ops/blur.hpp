//! \file
#ifndef Y_INK_BLUR_INCLUDED
#define Y_INL_BLUR_INCLUDED 1

#include "y/ink/pixmaps.hpp"
#include "y/container/matrix.hpp"

namespace upsylon
{
    namespace Ink
    {

        //! Gaussian Blur values
        class Blur : public Object
        {
        public:
            virtual ~Blur() throw();
            explicit Blur(const float sig);
            const float    sigma;   //!< gaussian deviation
            const float    scale;   //!< 2*sigma^2
            const unit_t   length;  //!< length >= sqrt(2*sigma^2*log(256))
            const unit_t   counts;  //!< 1+length
            matrix<float>  normal;  //!< counts*counts
            array<float>  &weight;  //!< counts

            template <typename SCALAR,typename TCOLOR, typename SCOLOR, const size_t CHANNELS, typename F2T>
            inline void _applyX( Pixmap<TCOLOR> &target, const Pixmap<SCOLOR> &source, F2T &float2type, Engine &E)
            {
                __applyX<SCALAR,TCOLOR,SCOLOR,CHANNELS,F2T> proxy = { this, &target, &source, &float2type };
                E.run(proxy);
            }

            template <typename SCALAR,typename TCOLOR, typename SCOLOR, const size_t CHANNELS, typename F2T>
            inline void _applyY( Pixmap<TCOLOR> &target, const Pixmap<SCOLOR> &source, F2T &float2type, Engine &E)
            {
                __applyY<SCALAR,TCOLOR,SCOLOR,CHANNELS,F2T> proxy = { this, &target, &source, &float2type};
                E.run(proxy);
            }

            template <typename SCALAR,typename TCOLOR, typename SCOLOR, const size_t CHANNELS, typename F2T>
            inline void _apply( Pixmap<TCOLOR> &target, Pixmap<TCOLOR> &temp, const Pixmap<SCOLOR> &source,F2T &float2type, Engine &E)
            {
                _applyX<SCALAR,TCOLOR,SCOLOR,CHANNELS,F2T>(temp,source,float2type,E);
                _applyY<SCALAR,TCOLOR,TCOLOR,CHANNELS,F2T>(target,temp,float2type,E);
            }

            void apply( Pixmap3 &target, Pixmap3 &temp, const Pixmap3 &source, Engine &E)
            {
                _apply<uint8_t,RGB,RGB,3>(target,temp,source,Crux::ClosestByte,E);
            }

            void apply( Pixmap3 &target, Pixmap3 &temp, const Pixmap4 &source, Engine &E)
            {
                _apply<uint8_t,RGB,RGBA,3>(target,temp,source,Crux::ClosestByte,E);
            }

            void apply( PixmapF &target, PixmapF &temp, const PixmapF &source, Engine &E)
            {
                _apply<float,float,float,1>(target,temp,source,Crux::ClosestFloat,E);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blur);
            template <typename SCALAR,typename TCOLOR, typename SCOLOR, const size_t CHANNELS,typename F2T>
            struct __applyX
            {
                const Blur           *_blur;
                Pixmap<TCOLOR>       *_target;
                const Pixmap<SCOLOR> *_source;
                F2T                  *_func;

                inline void operator()( const Area &area, lockable & )
                {
                    assert(_target);
                    assert(_source);
                    assert(_blur);
                    assert(_func);
                    if(!area.pixels) return;

                    const Pixmap<SCOLOR> &source = *_source;
                    Pixmap<TCOLOR>       &target = *_target;
                    const Blur           &blur   = *_blur;
                    F2T                  &func   = *_func;
                    const unit_t          length = blur.length;
                    const unit_t          xtop   = source.upper.x;

                    Y_INK_AREA_LIMITS(area);
                    const float         *w = *blur.weight;
                    const matrix<float> &n =  blur.normal;
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        //______________________________________________________
                        //
                        // compute parameters
                        //______________________________________________________
                        const unit_t xlo = max_of<unit_t>(x-length,0);
                        const unit_t xhi = min_of<unit_t>(x+length,xtop);
                        const unit_t ilo = xlo-x; assert(ilo<=0);
                        const unit_t ihi = xhi-x; assert(ihi>=0);
                        const float  nrm = n[ihi+1][-ilo+1];

                        for(unit_t y=ymax;y>=ymin;--y)
                        {
                            const typename Pixmap<SCOLOR>::Row &src = source[y];
                            float        sum[4] = { 0,0,0,0 };
                            for(unit_t xx=xhi,ii=ihi;xx>=xlo;--xx,--ii)
                            {
                                assert(abs_of(ii)<=length);
                                const SCALAR *p   = (const SCALAR *) (&src[xx]);
                                const float   fac = w[ abs_of(ii) ]; assert(fac<=1.0f);
                                for(size_t k=0;k<CHANNELS;++k)
                                {
                                    sum[k] += float(p[k]) * fac;
                                }
                            }
                            SCALAR *q = (SCALAR *) (&target[y][x]);
                            for(size_t k=0;k<CHANNELS;++k)
                            {
                                q[k] = SCALAR( func(sum[k]/nrm) );
                            }
                        }

                    }

                }
            };


            template <typename SCALAR,typename TCOLOR, typename SCOLOR, const size_t CHANNELS, typename F2T>
            struct __applyY
            {
                const Blur           *_blur;
                Pixmap<TCOLOR>       *_target;
                const Pixmap<SCOLOR> *_source;
                F2T                  *_func;

                inline void operator()( const Area &area, lockable & )
                {
                    assert(_target);
                    assert(_source);
                    assert(_blur);
                    assert(_func);
                    if(!area.pixels) return;

                    const Pixmap<SCOLOR> &source = *_source;
                    Pixmap<TCOLOR>       &target = *_target;
                    const Blur           &blur   = *_blur;
                    F2T                  &func   = *_func;
                    const unit_t          length = blur.length;
                    const unit_t          ytop   = source.upper.y;

                    Y_INK_AREA_LIMITS(area);
                    const float         *w = *blur.weight;
                    const matrix<float> &n =  blur.normal;
                    for(unit_t y=ymax;y>=ymin;--y)
                    {
                        //______________________________________________________
                        //
                        // compute parameters
                        //______________________________________________________
                        const unit_t ylo = max_of<unit_t>(y-length,0);
                        const unit_t yhi = min_of<unit_t>(y+length,ytop);
                        const unit_t jlo = ylo-y; assert(jlo<=0);
                        const unit_t jhi = yhi-y; assert(jhi>=0);
                        const float  nrm = n[jhi+1][-jlo+1];

                        typename Pixmap<TCOLOR>::Row &tgt = target[y];
                        for(unit_t x=xmax;x>=xmin;--x)
                        {
                            float        sum[4] = { 0,0,0,0 };
                            for(unit_t yy=yhi,jj=jhi;yy>=ylo;--yy,--jj)
                            {
                                const SCALAR *p   = (const SCALAR *) & source[yy][x];
                                const float   fac = w[ abs_of(jj) ]; assert(fac<=1.0f);
                                for(size_t k=0;k<CHANNELS;++k)
                                {
                                    sum[k] += float(p[k]) * fac;
                                }
                            }
                            SCALAR *q = (SCALAR *) (&tgt[x]);
                            for(size_t k=0;k<CHANNELS;++k)
                            {
                                q[k] = func( sum[k] / nrm );
                            }
                        }
                    }
                }
            };

        };

    }
}

#endif

