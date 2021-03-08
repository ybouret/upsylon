
#include "y/gfx/image/format/png.hpp"
#include "y/gfx/image/format/options.hpp"
#include "y/gfx/pblock.hpp"
#include "y/exception.hpp"

#include "y/png/png.h"

#include "y/memory/allocator/dyadic.hpp"
#include "y/ios/icstream.hpp"
#include "y/ios/ocstream.hpp"

namespace upsylon
{
    namespace graphic
    {

        png_format:: ~png_format() throw()
        {
        }

        const char * const png_format::ID = "PNG";

        png_format:: png_format() :
        image::named_format(ID, "png")
        {
        }

        namespace {

            typedef memory::dyadic PNG_Memory;

            class PNG_Mem
            {
            public:
                inline PNG_Mem() :
                rows(0),
                wlen(0),
                wksp(0)
                {
                    PNG_Memory::create();
                }

                inline ~PNG_Mem() throw()
                {
                    PNG_Memory::location().release(wksp, wlen);
                    rows=0;
                }

                png_bytep *rows;
                size_t     wlen;
                void      *wksp;

                void allocate( unit_t width, unit_t height, unit_t num_channels )
                {
                    const size_t rows_offset = 0;
                    const size_t rows_length = sizeof(png_bytep) * height;

                    const size_t data_offset = memory::align(rows_length+rows_offset);
                    const unit_t row_bytes   = num_channels * width;
                    const size_t data_length = height * row_bytes * sizeof(png_byte);

                    wlen = memory::align(data_offset+data_length);
                    wksp = PNG_Memory::instance().acquire(wlen);

                    uint8_t *addr = (uint8_t *)wksp;
                    rows = (png_bytep *)&addr[rows_offset];

                    uint8_t *data = &addr[data_offset];
                    for(unit_t i=0;i<height;++i)
                    {
                        rows[i] = (png_byte *)&data[i*row_bytes];
                    }

                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(PNG_Mem);
            };
        }



        bitmap png_format:: load_(const string         &file,
                                  const image::options *,
                                  const rgba_to_type   &conv) const
        {

            static const char fn[] = "png::load";
            Y_GIANT_LOCK();

            //__________________________________________________________________
            //
            // open file
            //__________________________________________________________________
            ios::icstream fp(file);

            //__________________________________________________________________
            //
            // read header
            //__________________________________________________________________
            {
                unsigned char header[8];
                const size_t  done = fp.try_query(header,8);
                if(done!=8)
                    throw exception("%s(missing header bytes)",fn);

                if (png_sig_cmp(header, 0, 8))
                    throw exception("%s(file is not recognized as PNG)", fn);
            }

            //__________________________________________________________________
            //
            // prepare to read
            //__________________________________________________________________
            png_structp png_ptr  = 0;
            png_infop   info_ptr = 0;

            png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if(!png_ptr)
                throw exception("%s(couldn't create PNG read struct)",fn);

            info_ptr = png_create_info_struct(png_ptr);
            if(!info_ptr)
            {
                png_destroy_read_struct(&png_ptr, NULL, NULL);
                throw exception("%s(could't create PNG info struct)",fn);
            }

            //__________________________________________________________________
            //
            //  Init I/O
            //__________________________________________________________________
            if (setjmp(png_jmpbuf(png_ptr)))
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                throw exception("%s(I/O failure)",fn);
            }

            png_init_io(png_ptr, *fp);
            png_set_sig_bytes(png_ptr, 8);

            png_read_info(png_ptr, info_ptr);

            const unit_t     width            = png_get_image_width(png_ptr, info_ptr);
            const unit_t     height           = png_get_image_height(png_ptr, info_ptr);
            //const png_byte   color_type       = png_get_color_type(png_ptr, info_ptr);
            png_byte         bit_depth        = png_get_bit_depth(png_ptr, info_ptr);
            const int        number_of_passes = png_set_interlace_handling(png_ptr);


            png_set_strip_16(png_ptr); //!< strip 16->8 bits
            png_set_packing(png_ptr);  //!< expand channels
            png_set_expand(png_ptr);   //!< expand all to RGBA...
            png_read_update_info(png_ptr, info_ptr);
            const unit_t num_channels = png_get_channels(png_ptr, info_ptr);
            bit_depth  = png_get_bit_depth(png_ptr, info_ptr);

            if(bit_depth!=8)
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                throw exception("%s(unstripped 16 bits!)",fn);
            }

            if(number_of_passes>1)
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                throw exception("%s(unhandled #passes=%d)",fn, number_of_passes);
            }

            switch(num_channels)
            {
                case 1:
                case 3:
                case 4:
                    break;

                default:
                    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                    throw exception("%s(unhandled #channels=%d)",fn, int(num_channels));
            }


            const unit_t     depth = conv.depth();
            auto_ptr<bitmap> bmp   = NULL;
            PNG_Mem          mem;
            try
            {
                bmp = new bitmap(width,height,depth);
                mem.allocate(width,height,num_channels);
            }
            catch(...)
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                throw;
            }

            //__________________________________________________________________
            //
            // read file
            //__________________________________________________________________
            if (setjmp(png_jmpbuf(png_ptr)))
            {
                png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
                throw exception("%s(I/O failure)",fn);
            }

            png_read_image(png_ptr,mem.rows);

            //__________________________________________________________________
            //
            // processing...
            //__________________________________________________________________
            unit_t y=height;
            for(unit_t j=0;j<height;++j)
            {
                const png_byte *q = mem.rows[j];
                uint8_t        *p = (uint8_t *)(bmp->get_line(--y));
                for(unit_t i=0;i<width;++i,q += num_channels , p += depth )
                {
                    switch(num_channels)
                    {
                        case 1: {
                            const rgba C(q[0],q[0],q[0],0xff);
                            conv(p,C);
                        } break;

                        case 3: {
                            const rgba C( q[0], q[1], q[2], 0xff);
                            conv(p,C);
                        } break;

                        case 4: {
                            const rgba C( q[0], q[1], q[2], q[3]);
                            conv(p,C);
                        } break;

                        default:
                            // never get here
                            ;
                    }
                }
            }

            //__________________________________________________________________
            //
            // cleanup
            //__________________________________________________________________
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            return *bmp;
        }

        static inline unit_t num_channels_for(const bool alpha) throw()
        {
            return alpha ? 4:3;
        }

        void   png_format:: save_(const bitmap         &bmp,
                                  const string         &file,
                                  const image::options *opts,
                                  const type_to_rgba   &conv) const
        {
            assert(conv.depth()==bmp.depth);
            static const char fn[] = "png::save";

            //__________________________________________________________________
            //
            // open file
            //__________________________________________________________________
            ios::ocstream        fp(file,false);

            //__________________________________________________________________
            //
            // parse options
            //__________________________________________________________________
            const bool     use_alpha    = image::options::flag(opts,"alpha");
            const unit_t   num_channels = num_channels_for(use_alpha);
            const int      zlevel       = unsigned(image::options::get<size_t>(opts,"z",6));
            if(zlevel>=10) throw exception("%s(invalid z=%u)",fn, unsigned(zlevel));

            //__________________________________________________________________
            //
            // prepare PNG
            //__________________________________________________________________
            png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

            if (!png_ptr)
            {
                throw exception("%s(can't init PNG)",fn);
            }

            png_infop   info_ptr = png_create_info_struct(png_ptr);
            if (!info_ptr)
            {
                png_destroy_write_struct(&png_ptr,NULL);
                throw exception("%s(can't init PNG info)",fn);
            }

            if (setjmp(png_jmpbuf(png_ptr)))
            {
                png_destroy_write_struct(&png_ptr,&info_ptr);
                throw exception("%s(init_io error)",fn);
            }

            png_init_io(png_ptr,*fp);

            //__________________________________________________________________
            //
            // write header
            //__________________________________________________________________
            if (setjmp(png_jmpbuf(png_ptr)))
            {
                png_destroy_write_struct(&png_ptr,&info_ptr);
                throw exception("%s(header error)",fn);
            }

            const uint32_t width  = uint32_t(bmp.w);
            const uint32_t height = uint32_t(bmp.h);
            png_set_IHDR(png_ptr, info_ptr,
                         width,
                         height,
                         8,
                         use_alpha ? PNG_COLOR_TYPE_RGB_ALPHA : PNG_COLOR_TYPE_RGB,
                         PNG_INTERLACE_NONE,
                         PNG_COMPRESSION_TYPE_BASE,
                         PNG_FILTER_TYPE_BASE);
            png_write_info(png_ptr, info_ptr);
            png_set_compression_level(png_ptr,zlevel);


            PNG_Mem mem;
            try
            {
                mem.allocate(width,height,num_channels);
            }
            catch(...)
            {
                png_destroy_write_struct(&png_ptr,&info_ptr);
                throw;
            }
            //__________________________________________________________________
            //
            // write bytes
            //__________________________________________________________________
            if (setjmp(png_jmpbuf(png_ptr)))
            {
                png_destroy_write_struct(&png_ptr,&info_ptr);
                throw exception("%s(write_image error)", fn);
            }

            unit_t       y     = height;
            const unit_t depth = bmp.depth;

            for(uint32_t j=0;j<height;++j)
            {
                const uint8_t *p = (const uint8_t *)(bmp.get_line(--y));
                png_byte      *q = mem.rows[j];
                for(uint32_t i=0;i<width;++i, p+=depth, q += num_channels)
                {
                    const rgba C = conv(p);
                    q[0] = C.r;
                    q[1] = C.g;
                    q[2] = C.b;
                    if(use_alpha)
                    {
                        q[3] = C.a;
                    }
                }
            }

            png_write_image(png_ptr, mem.rows);


            //__________________________________________________________________
            //
            // end write
            //__________________________________________________________________
            if (setjmp(png_jmpbuf(png_ptr)))
            {
                png_destroy_write_struct(&png_ptr,&info_ptr);
                throw exception("%s(write_end error)",fn);
            }

            png_write_end(png_ptr, NULL);

            //__________________________________________________________________
            //
            // normal return
            //__________________________________________________________________
            png_destroy_write_struct(&png_ptr,&info_ptr);
        }
    }

}

