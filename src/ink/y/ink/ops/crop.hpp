//! \file

#ifndef Y_INK_OPS_CROP_INCLUDED
#define Y_INK_OPS_CROP_INCLUDED 1

#include "y/ink/pixmaps.hpp"
#include "y/ink/pixel.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! function to dectect crop
        struct Crop
        {
            
            //! find zero area on the left
            template <typename T> static inline
            size_t Left( const Pixmap<T> &pxm ) throw() { return HScan(pxm,LeftZero<T>); }

            //! find zero area on teh right
            template <typename T> static inline
            size_t Right( const Pixmap<T> &pxm ) throw() { return HScan(pxm,RightZero<T>); }



        private:
            template <typename T,typename ZCOUNT> static inline
            size_t HScan( const Pixmap<T> &pxm, ZCOUNT &zcount ) throw()
            {
                size_t nz = zcount(pxm,0);
                if(nz>0)
                {
                    const size_t h=pxm.h;
                    for(size_t j=1;j<h;++j)
                    {
                        const size_t tmp = zcount(pxm,j);
                        if(tmp<nz)
                        {
                            nz = tmp;
                            if(nz<=0) return 0;
                        }
                    }
                    return nz;
                }
                else
                {
                    return 0;
                }
            }

            template <typename T>
            static inline size_t LeftZero( const Pixmap<T> &pxm, const unit_t j) throw()
            {
                const typename Pixmap<T>::Row &r = pxm[j];
                const size_t                   w = pxm.w;
                size_t count=0;
                for(size_t i=0;i<w;)
                {
                    if( Pixel<T>::IsZero(r[i++]) ) ++count; else break;
                }
                return count;
            }

            template <typename T>
            static inline size_t RightZero( const Pixmap<T> &pxm, const unit_t j) throw()
            {
                const typename Pixmap<T>::Row &r = pxm[j];
                size_t count=0;
                for(size_t i=pxm.w;i>0;)
                {
                    if( Pixel<T>::IsZero(r[--i]) ) ++count; else
                    {
                        //std::cerr << "@" << j << "+" << i << " : " << r[i] << std::endl;
                        break;
                    }
                }
                //std::cerr << "right@" << j << ": " << count << std::endl;
                return count;
            }


        };
    }
}

#endif


