#include "y/gfx/image/format/tiff.hpp"
#include "y/ptr/auto.hpp"


namespace upsylon
{
    namespace graphic
    {
        
        const char tiff_format::ID[] = "TIFF";
        
        tiff_format:: ~tiff_format() throw()  {}
        
        
        tiff_format:: tiff_format() : named_format(ID,"tiff?"), raster() {}
        
        
        
#define TIFFGetR(abgr) ( (abgr)        & 0xff)
#define TIFFGetG(abgr) (((abgr) >> 8)  & 0xff)
#define TIFFGetB(abgr) (((abgr) >> 16) & 0xff)
#define TIFFGetA(abgr) (((abgr) >> 24) & 0xff)
        
        bitmap tiff_format:: load_(const string         &file,
                                   const image::options *opts,
                                   const rgba_to_type   &conv) const
        {
            // open file
            I_TIFF      tiff(file);
            tiff.SetDirectory( image::options::get<size_t>(opts,"directory",0));
            
            
            
            // read raster
            tiff.ReadRBGAImage(raster);
            
            // allocate resources
            const int w = tiff.GetWidth();
            const int h = tiff.GetHeight();
            bitmap    bmp(w,h,conv.depth());
            expand(bmp,raster,conv);
            return bmp;
            
        }
        
        void tiff_format:: expand(bitmap              &bmp,
                                  const _TIFF::Raster &raster,
                                  const rgba_to_type  &proc)
        {
            assert(bmp.depth==proc.depth());
            const unit_t w = bmp.w;
            const unit_t h = bmp.h;
            assert( raster.ssize() >= (w*h) );
            const uint32_t *p = *raster;
            for(int j=0;j<h;++j)
            {
                const uint32_t *q = &p[j*w];
                for(int i=0;i<w;++i)
                {
                    const uint32_t P = *(q++);
                    const rgba     C( TIFFGetR(P), TIFFGetG(P), TIFFGetB(P), TIFFGetA(P));
                    proc( bmp.at(i,j),C);
                }
            }
        }
        
        
         void  tiff_format:: save_(const bitmap         &bmp,
                                  const string         &file,
                                  const image::options *,
                                  const type_to_rgba   &conv) const
        {
            // open file
            O_TIFF tiff(file,false);
            
            
            // compile data
            compile(raster,bmp,conv);
            
            // call library
            tiff.WriteRGBAImage(raster, int(bmp.w), int(bmp.h), 0);
            
            
        }
        
        void tiff_format:: compile(_TIFF::Raster      &raster,
                                   const bitmap       &bmp,
                                   const type_to_rgba &proc)
        {
            assert(bmp.depth==proc.depth());
            const unit_t w = bmp.w;
            const unit_t h = bmp.h;
            raster.startup(w*h);
            uint32_t *p = *raster;
            for(unit_t j=0;j<h;++j)
            {
                uint32_t *q = &p[j*w];
                for(unit_t i=0;i<w;++i)
                {
                    const rgba C = proc(bmp.at(i,j));
                    uint32_t  &Q = *(q++);
                    Q  = C.a; Q <<= 8;
                    Q |= C.b; Q <<= 8;
                    Q |= C.g; Q <<= 8;
                    Q |= C.r;
                    assert( TIFFGetR(Q) == C.r );
                    assert( TIFFGetG(Q) == C.g );
                    assert( TIFFGetB(Q) == C.b );
                    assert( TIFFGetA(Q) == C.a );
                }
            }
        }
        
        
    }
}


