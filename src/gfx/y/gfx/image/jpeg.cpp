
#include "y/gfx/image/jpeg.hpp"
#include "y/gfx/pblock.hpp"
#include "y/exception.hpp"
#include "y/memory/allocator/dyadic.hpp"

extern "C" {
#include "y/jpeg/jpeglib.h"
}

#include <cstring>
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/exception.hpp"
#include <setjmp.h>


namespace upsylon
{
    namespace graphic
    {

        jpeg_format:: ~jpeg_format() throw()
        {
        }

        jpeg_format:: jpeg_format() :
        image::named_format( "JPG", "jpe?g" )
        {
        }


        namespace {
            struct my_error_mgr
            {
                struct jpeg_error_mgr pub;           /* "public" fields */
                jmp_buf               setjmp_buffer; /* for return to caller */
            };

            typedef struct my_error_mgr * my_error_ptr;


            METHODDEF(void)
            my_error_exit (j_common_ptr cinfo)
            {
                /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
                my_error_ptr myerr = (my_error_ptr) cinfo->err;

                /* Always display the message. */
                /* We could postpone this until after returning, if we chose. */
                (*cinfo->err->output_message) (cinfo);

                /* Return control to the setjmp point */
                longjmp(myerr->setjmp_buffer, 1);
            }




        }


        //typedef zblock<JSAMPLE, memory::dyadic> jsample_buffer;
        typedef pblock<JSAMPLE> jsample_buffer;
        
        bitmap jpeg_format:: load(const string       &file,
                                  const void         *opts,
                                  const rgba_to_type &conv)
        {

            static const char fn[] = "jpeg::load";
            Y_GIANT_LOCK();

            ios::icstream                 fp(file);
            struct jpeg_decompress_struct cinfo;
            struct my_error_mgr           jerr;

            memset(&cinfo,0,sizeof(cinfo));
            memset(&jerr,0,sizeof(jerr));

            //__________________________________________________________________
            //
            // We set up the normal JPEG error routines,
            // then override error_exit
            //__________________________________________________________________
            cinfo.err           = jpeg_std_error(&jerr.pub);
            jerr.pub.error_exit = my_error_exit;

            //__________________________________________________________________
            //
            // Establish the setjmp return context for my_error_exit to use.
            //__________________________________________________________________
            if (setjmp(jerr.setjmp_buffer)) {
                /* If we get here, the JPEG code has signaled an error.
                 * We need to clean up the JPEG object, close the input file, and return.
                 */
                jpeg_destroy_decompress(&cinfo);
                throw exception("%s(failure)",fn);
            }

            //__________________________________________________________________
            //
            // Now we can initialize the JPEG decompression object
            //__________________________________________________________________
            jpeg_create_decompress(&cinfo);

            //__________________________________________________________________
            //
            // Step 2: specify data source (eg, a file)
            //__________________________________________________________________
            jpeg_stdio_src(&cinfo, *fp);


            //__________________________________________________________________
            //
            // Read file parameters with jpeg_read_header()
            //__________________________________________________________________


            (void) jpeg_read_header(&cinfo, TRUE);
            /* We can ignore the return value from jpeg_read_header since
             *   (a) suspension is not possible with the stdio data source, and
             *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
             * See libjpeg.doc for more info.
             */

            //__________________________________________________________________
            //
            // Step 5: Start decompressor
            //__________________________________________________________________

            (void) jpeg_start_decompress(&cinfo);
            /* We can ignore the return value since suspension is not possible
             * with the stdio data source.
             */

            try
            {
                const unit_t width  = cinfo.output_width;
                const unit_t height = cinfo.output_height;
                const unit_t depth  = conv.depth();
                //std::cerr << "width=" << width << ", height=" << height << std::endl;
                if(width<=0||height<=0)
                    throw exception("%s(invalid witdh/height)", fn);

                if(cinfo.output_components!=3)
                    throw exception("%s(unsupported image depth=%d)",fn,int(cinfo.output_components));

                bitmap bmp(width,height,depth);

                jsample_buffer jsbuff(cinfo.output_width * cinfo.output_components);
                JSAMPLE       *buffer = &jsbuff[0];

                //buflen = cinfo.output_width * cinfo.output_components;
                //buffer = memory::global::instance().acquire_as<JSAMPLE>(buflen,_bufsz);

                unit_t j = height-1;
                while (cinfo.output_scanline < cinfo.output_height)
                {
                    (void) jpeg_read_scanlines(&cinfo, &buffer, 1);
                    uint8_t *p = static_cast<uint8_t*>(bmp.get_line(j));
                    for(unit_t i=0;i<width;++i)
                    {
                        const unit_t   i3 = 3*i;
                        const JSAMPLE *b  = &buffer[i3];
                        const rgba     C(b[0],b[1],b[2],0xff);
                        conv(p,C);
                        p += depth;
                    }
                    --j;
                }

                /* Step 7: Finish decompression */

                (void) jpeg_finish_decompress(&cinfo);
                /* We can ignore the return value since suspension is not possible
                 * with the stdio data source.
                 */
                jpeg_destroy_decompress(&cinfo);
                return bmp;
            }
            catch(...)
            {
                jpeg_destroy_decompress(&cinfo);
                throw;
            }
            throw exception("not implemented");
        }

        void jpeg_format:: save(const bitmap       &bmp,
                                const string       &file,
                                const void         *opts,
                                const type_to_rgba &conv)
        {
            throw exception("not implemented");
        }

    }

}


