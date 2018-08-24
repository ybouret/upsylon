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
    namespace ink
    {

        //! interface for image Input/Output
        class imageIO : public counted_object
        {
        public:
            const string name; //!< identifier for databse
            //! destructor
            inline virtual ~imageIO() throw() {}
            //! key=name for database
            inline const string & key() const throw() { return name; }

            //==================================================================
            //
            // virtual interface
            //
            //==================================================================

            //! load a bitmap with proc converting RGBA to something sizeof=depth
            virtual bitmap  *load(const string          &filename,
                                  unit_t                 depth,
                                  rgba2data             &proc,
                                  const void            *options) const = 0;

            //! save a bitmap with proc converting each pixel into RGBA
            virtual void     save(const string        &filename,
                                  const bitmap        &bmp,
                                  data2rgba           &proc,
                                  const void          *options) const = 0;


            //==================================================================
            //
            // non-virtual interface : load
            //
            //==================================================================


            //! load a floating point grey scale bitmap
            inline pixmapf loadf(const string &filename, const void *options) const
            {
                put_gsf proc;
                return pixmapf(load(filename,sizeof(float),proc,options));
            }

            //! load a 8-bits grey-scale bitmap
            inline pixmap1 load1(const string &filename, const void *options) const
            {
                put_gsu proc;
                return pixmap1(load(filename,sizeof(uint8_t),proc,options));
            }

            //! load an rgb bitmap
            inline pixmap3 load3(const string &filename, const void *options) const
            {
                put_rgb proc;
                return pixmap3(load(filename,sizeof(RGB),proc,options));
            }

            //! load an rgba bitmap
            inline pixmap4 load4(const string &filename, const void *options) const
            {
                put_rgba proc;
                return pixmap4(load(filename,sizeof(RGBA),proc,options));
            }

            //==================================================================
            //
            // non-virtual interface : save
            //
            //==================================================================

            //! save pixmap<float>
            inline void save(const string &filename, const pixmap<float> &bmp, const void *options) const
            {
                get_from_float proc;
                save(filename,bmp,proc,options);
            }

            //! save pixmap<uint8_t>
            inline void save(const string &filename, const pixmap<uint8_t> &bmp, const void *options) const
            {
                get_from_byte proc;
                save(filename,bmp,proc,options);
            }

            //! save pixmap<RGB>
            inline void save(const string &filename, const pixmap<RGB> &bmp, const void *options) const
            {
                get_from_rgb proc;
                save(filename,bmp,proc,options);
            }

            //! save pixmap<RGBA>
            inline void save(const string &filename, const pixmap<RGBA> &bmp, const void *options) const
            {
                get_from_rgba proc;
                save(filename,bmp,proc,options);
            }

        protected:
            //! initialize
            inline explicit imageIO(const char *id) : name(id) {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(imageIO);
        };

        //! handling different imageIO
        class image : public imageIO, public singleton<image>
        {
        public:

            //! a format is an imageIO plus info
            class format : public imageIO
            {
            public:
                typedef intr_ptr<string,format> pointer;  //!< held in set
                typedef set<string,pointer>     database; //!< database of formats, by nmae
                typedef map<string,pointer>     exttable; //!< database of formats, by extensions

                //! information
                virtual bool         lossless()   const throw() = 0;
                //! table of possible extensions
                virtual const char **extensions() const throw() = 0;
                //! destructor
                inline virtual ~format() throw() {}

            protected:
                //! initialize
                inline explicit format(const char *id) : imageIO(id) {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(format);
            };

            //! enrolling a new format
            void enroll( const format::pointer &fmt );

            //! low level creation, register the format only
            template <typename  FMT>
            FMT & create()
            {
                FMT *fmt = new FMT();
                const format::pointer format(fmt);
                enroll(fmt);
                return  *fmt;
            }
            
            virtual bitmap  *load(const string          &filename,
                                  unit_t                 depth,
                                  rgba2data             &proc,
                                  const void            *options) const;

            virtual void     save(const string        &filename,
                                  const bitmap        &bmp,
                                  data2rgba           &proc,
                                  const void          *options) const;

            //! enroll default format
            image &initialize();

            //! display supported extension
            void display() const;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(image);
            friend class singleton<image>;

            format::database fmt_db;
            format::exttable ext_db;

            explicit image();
            virtual ~image() throw();
            static const at_exit::longevity life_time = object::life_time-10;

            format & get_format_for(const string &filename) const;
        };
    }
}

#endif

