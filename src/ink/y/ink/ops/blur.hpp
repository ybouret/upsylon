//! \file
#ifndef Y_INK_BLUR_INCLUDED
#define Y_INL_BLUR_INCLUDED 1

#include "y/ink/pixmap.hpp"
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

            template <typename SCALAR,typename TCOLOR, typename SCOLOR, const size_t CHANNELS>
            void applyX( Pixmap<TCOLOR> &target, const Pixmap<SCOLOR> &source,Engine E)
            {
                __applyX<SCALAR,TCOLOR,SCOLOR,CHANNELS> proxy = { this, &target, &source };
                E.run(proxy);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Blur);
            template <typename SCALAR,typename TCOLOR, typename SCOLOR, const size_t CHANNELS>
            struct __applyX
            {
                const Blur           *_blur;
                Pixmap<TCOLOR>       *_target;
                const Pixmap<SCOLOR> *_source;

                inline void operator()( const Area &area, lockable & )
                {
                    assert(_target);
                    assert(_source);
                    if(!area.pixels) return;

                    const Pixmap<SCOLOR> &source = *_source;
                    Pixmap<TCOLOR>       &target = *_target;
                    const Blur           &blur   = *_blur;
                    const unit_t          length = blur.length;
                    const unit_t          xtop   = source.upper.x;

                    Y_INK_AREA_LIMITS(area);
                    const float         *w = *blur.weight;
                    const matrix<float> &n =  blur.normal;
                    for(unit_t x=xmax;x>=xmin;--x)
                    {
                        const unit_t xlo = max_of<unit_t>(x-length,0);
                        const unit_t xhi = min_of<unit_t>(x+length,xtop);
                        const unit_t ilo = xlo-x; assert(ilo<=0);
                        const unit_t ihi = xhi-x; assert(ihi>=0);
                        const float  nrm = n[ihi+1][-ilo+1];
                        float        sum[4] = { 0,0,0,0 };
                        for(unit_t y=ymax;y>=ymin;--y)
                        {
                            //typename Pixmap<TCOLOR>::row       &tgt = target[y];
                            const typename Pixmap<SCOLOR>::row &src = source[y];
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
                            std::cerr << "@[" << y << "][" << x << "]:";
                            for(size_t k=0;k<CHANNELS;++k)
                            {
                                q[k] = SCALAR( floorf( sum[k] / nrm + 0.5f ) );
                                std::cerr << ' ' << double(q[k]);
                                //const SCALAR *p   = (const SCALAR *) (&src[x]);
                                //q[k] = p[k];
                                break;
                            }
                            std::cerr << std::endl;
                        }
                    }

                }
            };
        };

    }
}

#endif

