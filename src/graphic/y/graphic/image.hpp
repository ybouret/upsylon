
//! \file

#ifndef Y_GRAPHIC_IMAGE_INCLUDED
#define Y_GRAPHIC_IMAGE_INCLUDED 1

#include "y/graphic/pixmap.hpp"
#include "y/graphic/image/pixtags.hpp"
#include "y/graphic/color/rgba2data.hpp"
#include "y/graphic/color/data2rgba.hpp"
#include "y/string/convert.hpp"
#include "y/associative/map.hpp"
#include "y/ordered/sorted-vector.hpp"

namespace upsylon {

    namespace Graphic {

        //! Images and Formats management
        class Image : public singleton<Image>
        {
        public:
            typedef key_hasher<string,hashing::fnv>                  KeyHasher;         //!< alias
            typedef memory::pooled                                   Allocator;         //!< alias
            typedef map<string,string,KeyHasher,Allocator>           OptionDB;          //!< alias
            typedef increasing_comparator<string>                    StringComparator;  //!< alias
            typedef sorted_vector<string,StringComparator,Allocator> SortedStrings;     //!< alias
            typedef ordered_single<SortedStrings>                    OrderedStrings;    //!< alias

            //! options for format
            class Options : public OptionDB
            {
            public:
                Options() throw();          //!< setup
                Options(const char   *);    //!< setup and parse input
                Options(const string &);    //!< setup and parse input
                virtual ~Options() throw(); //!< cleanup
                Options(const Options &);   //!< copy

                Options & parse( const string & ); //!< parse input
                Options & parse( const char   * ); //!< parse input

                Options & operator<<( const string & ); //!< helper
                Options & operator<<( const char   * ); //!< helper

                bool flag( const string &name ) const;  //!< name must be a declared flag

                //! query and convert, default value otherwise
                template <typename T>
                inline T get( const string &name, const T defaultValue=0) const
                {
                    const string *pValue = this->search(name);
                    if( pValue )
                    {
                        return string_convert::to<T>(*pValue,*name);
                    }
                    else
                    {
                        return defaultValue;
                    }
                }

                //! test optional options for a flag
                static bool Flag(const Options *options, const string &name);
                //! test optional options for a flaf
                static bool Flag(const Options *options, const char   *name);


                //! test optional options, return defaultValue otherwise
                template <typename T> static inline
                T Get(const Options *options, const string &name, const T defaultValue=0)
                {
                    if(options)
                    {
                        return options->get<T>(name,defaultValue);
                    }
                    else
                    {
                        return defaultValue;
                    }
                }

                //! Get wrapper
                template <typename T> static inline
                T Get(const Options *options, const char *name, const T defaultValue=0)
                {
                    const string _(name);
                    return Get<T>(options,_,defaultValue);
                }

            private:
                Y_DISABLE_ASSIGN(Options);
            };

            
            //! interface for an image format
            class Format : public Object
            {
            public:
                const string name; //!< name for database of formats

                typedef intr_ptr<string,Format>                 Pointer; //!< alias for db
                typedef set<string,Pointer,KeyHasher,Allocator> Set;     //!< alias

                virtual ~Format() throw(); //!< cleanup

                //! load a new bitmap
                virtual Bitmap *load(const string  &filename,
                                     const size_t  depth,
                                     RGBA2Data     &proc,
                                     const Options *params) const = 0;

                //! save a bitmap
                virtual void    save(const string  &filename,
                                     const Bitmap  &bmp,
                                     Data2RGBA     &proc,
                                     const Options *params) const = 0;

                const string &key() const throw(); //!< key for set


            protected:
                explicit Format(const char *id, const char **ext); //!< setup
                OrderedStrings extensions; //!< recognized extensions

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Format);
                friend class Image;
            };


            static const at_exit::longevity life_time = 0; //!< longevity

            void use( Format *format ); //!< declare format to use

            //! get format according to file extension
            const Format & FormatFor(const string &filename) const;

            //! get format an load
            Bitmap *load(const string  &filename,
                         const size_t  depth,
                         RGBA2Data     &proc,
                         const Options *params) const;

            //! get format and save
            void    save(const string  &filename,
                         const Bitmap  &bmp,
                         Data2RGBA     &proc,
                         const Options *params) const;

            //! default loading
            template <typename T> inline
            Bitmap *loadAs(const string &filename, const Options *params=NULL) const
            {
                PutRGBA<T> proc;
                return load(filename,sizeof(T),proc,params);
            }

            //! default saving
            template <typename T> inline
            void saveAs(const string    &filename,
                        const Pixmap<T> &pxm,
                        const Options   *params) const
            {
                GetRGBA<T> proc;
                save(filename,*pxm,proc,params);
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Image);
            explicit Image();
            virtual ~Image() throw();
            typedef arc_ptr<Format> Fmt;
            typedef map<string,Fmt,KeyHasher,Allocator> FmtDB;

            Format::Set formats;
            FmtDB       db;

            friend class singleton<Image>;
            void compile();

        public:
            PixTags tags; //!< pixel tags for I/O
        };


    }
}

#endif

