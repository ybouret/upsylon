

//! \file

#ifndef Y_GFX_OPS_INTENSITY_INCLUDED
#define Y_GFX_OPS_INTENSITY_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/color/convert.hpp"
#include "y/gfx/pixel.hpp"

namespace upsylon
{
    namespace graphic
    {

        //______________________________________________________________________
        //
        //! handle intensites
        //______________________________________________________________________
        class intensity
        {
        public:
            intensity() throw();          //!< setup
            virtual ~intensity() throw(); //!< cleanup

            const float vmin; //!< minimal intensity
            const float vmax; //!< maximal intensity
            const float scal; //!< scaling

            //! compute vmin/vmax/scal
            template <typename T> inline
            void scan(const pixmap<T> &source, broker &apply)
            {
                apply.caches.make<float>(4);
                apply(kernel_scan<T>,(void*)&source);
                load(apply);
            }

            //! direct enhancement
            template <typename T> inline
            void enhance(pixmap<T>       &target,
                         const pixmap<T> &source,
                         broker          &apply) const
            {
                if(scal>0.0f)
                {
                    struct ops
                    {
                        pixmap<T>       &target;
                        const pixmap<T> &source;
                        const intensity &data;

                        static inline
                        void run(const tile &t, void *args, lockable &) throw()
                        {
                            ops             &self   = *static_cast<ops*>(args);
                            pixmap<T>       &target = self.target;
                            const pixmap<T> &source = self.source;
                            const intensity &data   = self.data;
                            const float      vmin   = data.vmin;
                            const float      vmax   = data.vmax;
                            const float      scal   = data.scal;
                            const T          zpix   = pixel::zero<T>();

                            for(size_t j=t.size();j>0;--j)
                            {
                                const segment   &s     = t[j];
                                const unit_t     y     = s.y;
                                const pixrow<T> &src_y = source(y);
                                pixrow<T>       &tgt_y = target(y);
                                const unit_t     xmin = s.xmin;

                                for(unit_t x=s.xmax;x>=xmin;--x)
                                {
                                    const T    &src  = src_y(x);
                                    T          &tgt  = tgt_y(x);
                                    const float vcur = convert<float,T>::from( src ); // current intensity
                                    if(vcur<=vmin)
                                    {
                                        tgt = zpix;
                                    }
                                    else
                                    {
                                        const T pmax = pixel::saturated(src);
                                        assert(( convert<float,T>::from(pmax) >= vcur) );
                                        if(vcur>=vmax)
                                        {
                                            tgt = pmax;
                                        }
                                        else
                                        {
                                            const float vnew = clamp<float>(0.0f,scal*(vcur-vmin),1.0f);
                                            tgt = pixel::mul_by(vnew,pmax);
                                        }
                                    }
                                }
                            }
                        }

                    };

                    ops todo = { target, source, *this };
                    apply(ops::run,&todo);

                }
                else
                {
                    target.ldz();
                }
            }


        private:
            template <typename T> static inline
            void kernel_scan(const tile &t, void *args, lockable &) throw()
            {
                const pixmap<T> &pxm  = *static_cast< const pixmap<T> *>(args);
                float            vmin = convert<float,T>::from(pxm( t.origin() ));
                float            vmax = vmin;
                for(size_t j=t.size();j>0;--j)
                {
                    const segment   &s    = t[j];
                    const pixrow<T> &r    = pxm(s.y);
                    const unit_t     xmin = s.xmin;

                    for(unit_t x=s.xmax;x>=xmin;--x)
                    {
                        const float tmp = convert<float,T>::from( r(x) );
                        if(tmp<vmin) vmin = tmp;
                        if(tmp>vmax) vmax = tmp;
                    }
                }
                assert(t.cache->is<float>());
                assert(t.cache->tell()>=2);
                t.cache->get<float>(1) = vmin;
                t.cache->get<float>(2) = vmax;
            }

            void load(const broker &apply) throw();

            
        };

    }

}

#endif

