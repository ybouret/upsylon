#include "y/graphic/image/tiff.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace Graphic
    {
        
        TIFF_Format:: ~TIFF_Format() throw()
        {
        }

        static const char * tiff_extensions[] = 
        {
            "tif",
            "tiff",
            NULL
        };

        TIFF_Format:: TIFF_Format() :
        Image::Format("TIFF",tiff_extensions),
        raster()
        {
        }
        


#define TIFFGetR(abgr) ( (abgr)        & 0xff)
#define TIFFGetG(abgr) (((abgr) >> 8)  & 0xff)
#define TIFFGetB(abgr) (((abgr) >> 16) & 0xff)
#define TIFFGetA(abgr) (((abgr) >> 24) & 0xff)

        Bitmap  *TIFF_Format:: load(const string         &filename,
                                    const size_t          depth,
                                    RGBA2Data            &proc,
                                    const Image::Options *params) const
        {
            // open file
            I_TIFF      tiff(filename);
            tiff.SetDirectory( Image::Options::Get<size_t>(params,"directory",0));

            

            // read raster
            tiff.ReadRBGAImage(raster);
            
            // allocate resources
            const int        w = tiff.GetWidth();
            const int        h = tiff.GetHeight();
            auto_ptr<Bitmap> B = Bitmap::Create(w, h, depth);


            // map raster to bitmap
            const uint32_t *p = *raster;
            for(int j=0;j<h;++j)
            {
                const uint32_t *q = &p[j*w];
                for(int i=0;i<w;++i)
                {
                    const uint32_t P = *(q++);
                    const rgba     C( TIFFGetR(P), TIFFGetG(P), TIFFGetB(P), TIFFGetA(P));
                    proc( (void*)(B->stdGet(i,j)),C);
                }
            }
            
            return B.yield();
            
            
            
        }

        size_t TIFF_Format:: countDirectories(const string &filename) const
        {
            I_TIFF tiff(filename);
            return tiff.CountDirectories();
        }

        

        void TIFF_Format:: save(const string         &filename,
                                const Bitmap         &bmp,
                                Data2RGBA            &proc,
                                const Image::Options *options) const
        {
            // open file
            O_TIFF tiff(filename,options,false);


            // compile data
            raster.compileBitmap(bmp, proc);
            
            // call library
            tiff.WriteRGBAImage(raster, bmp.w, bmp.h, 0);


        }

        
        
    }
}
