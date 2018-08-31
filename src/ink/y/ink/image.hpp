//! \file
#ifndef Y_INK_IMAGE_INCLUDED
#define Y_INK_IMAGE_INCLUDED 1

#include "y/ink/color/rgba2data.hpp"
#include "y/ink/color/data2rgba.hpp"
#include "y/ink/pixmaps.hpp"
#include "y/string.hpp"
#include "y/associative/set.hpp"
#include "y/associative/map.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon
{
    namespace Ink
    {

        //! interface for image Input/Output
        class ImageIO : public Object, public counted
        {
        public:
            const string name; //!< identifier for databse

            //! destructor
            inline virtual ~ImageIO() throw() {}

            //! key=name for database
            inline const string & key() const throw() { return name; }

            //==================================================================
            //
            // virtual interface
            //
            //==================================================================

            //! load a bitmap with proc converting RGBA to something sizeof=depth
            virtual Bitmap  *load(const string          &filename,
                                  unit_t                 depth,
                                  rgba2data             &proc,
                                  const void            *options) const = 0;

            //! save a bitmap with proc converting each pixel into RGBA
            virtual void     save(const string        &filename,
                                  const Bitmap        &bmp,
                                  data2rgba           &proc,
                                  const void          *options) const = 0;


            //==================================================================
            //
            // non-virtual interface : load
            //
            //==================================================================


            //! load a floating point grey scale bitmap
            inline PixmapF loadf(const string &filename, const void *options) const
            {
                put_gsf proc;
                return PixmapF(load(filename,sizeof(float),proc,options));
            }

            //! load a 8-bits grey-scale bitmap
            inline Pixmap1 load1(const string &filename, const void *options) const
            {
                put_gs1 proc;
                return Pixmap1(load(filename,sizeof(uint8_t),proc,options));
            }

            //! load an rgb bitmap
            inline Pixmap3 load3(const string &filename, const void *options) const
            {
                put_rgb proc;
                return Pixmap3(load(filename,sizeof(RGB),proc,options));
            }

            //! load an rgba bitmap
            inline Pixmap4 load4(const string &filename, const void *options) const
            {
                put_rgba proc;
                return Pixmap4(load(filename,sizeof(RGBA),proc,options));
            }

            //==================================================================
            //
            // non-virtual interface : save
            //
            //==================================================================

            //! save pixmap<float>
            inline void save(const string &filename, const Pixmap<float> &bmp, const void *options) const
            {
                get_from_float proc;
                save(filename,bmp,proc,options);
            }

            //! save pixmap<uint8_t>
            inline void save(const string &filename, const Pixmap<uint8_t> &bmp, const void *options) const
            {
                get_from_byte proc;
                save(filename,bmp,proc,options);
            }

            //! save pixmap<RGB>
            inline void save(const string &filename, const Pixmap<RGB> &bmp, const void *options) const
            {
                get_from_rgb proc;
                save(filename,bmp,proc,options);
            }

            //! save pixmap<RGBA>
            inline void save(const string &filename, const Pixmap<RGBA> &bmp, const void *options) const
            {
                get_from_rgba proc;
                save(filename,bmp,proc,options);
            }

        protected:
            //! initialize
            inline explicit ImageIO(const char *id) : name(id) {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ImageIO);
        };

        //! handling different imageIO
        class Image : public ImageIO, public singleton<Image>
        {
        public:

            //! a format is an imageIO plus info
            class Format : public ImageIO
            {
            public:
                typedef intr_ptr<string,Format> Pointer;  //!< held in set
                typedef set<string,Pointer>     Database; //!< database of formats, by nmae
                typedef map<string,Pointer>     ExtTable; //!< database of formats, by extensions

                //! information
                virtual bool         lossless()   const throw() = 0;
                //! table of possible extensions
                virtual const char **extensions() const throw() = 0;
                //! destructor
                inline virtual ~Format() throw() {}

            protected:
                //! initialize
                inline explicit Format(const char *id) : ImageIO(id) {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Format);
            };

            //! enrolling a new format
            void enroll( const Format::Pointer &fmt );

            //! low level creation, register the format only
            template <typename  FMT>
            FMT & create()
            {
                FMT *fmt = new FMT();
                const Format::Pointer f(fmt);
                enroll(f);
                return  *fmt;
            }
            
            virtual Bitmap  *load(const string          &filename,
                                  unit_t                 depth,
                                  rgba2data             &proc,
                                  const void            *options) const;

            virtual void     save(const string        &filename,
                                  const Bitmap        &bmp,
                                  data2rgba           &proc,
                                  const void          *options) const;

            //! enroll default format
            Image &initialize();

            //! return inialized instance
            static inline Image & Init()
            {
                return instance().initialize();
            }

            //! display supported extension
            void display() const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Image);
            friend class singleton<Image>;

            Format::Database fmt_db;
            Format::ExtTable ext_db;

            explicit Image();
            virtual ~Image() throw();
            static const at_exit::longevity life_time = object::life_time-10;

            Format & get_format_for(const string &filename) const;
        };
    }
}

#endif

