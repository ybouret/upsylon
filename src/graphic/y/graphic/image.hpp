
//! \file

#ifndef Y_GRAPHIC_IMAGE_INCLUDED
#define Y_GRAPHIC_IMAGE_INCLUDED 1

#include "y/graphic/bitmap.hpp"
#include "y/graphic/color/rgba2data.hpp"
#include "y/graphic/color/data2rgba.hpp"
#include "y/string/convert.hpp"
#include "y/associative/map.hpp"
#include "y/associative/set.hpp"
#include "y/memory/pooled.hpp"
#include "y/ptr/intr.hpp"
#include "y/ordered/sorted-vector.hpp"

namespace upsylon {

    namespace Graphic {


        //! Images and Formats management
        class Image : public singleton<Image>
        {
        public:
            typedef key_hasher<string,hashing::fnv>                 KeyHasher;
            typedef memory::pooled                                   Allocator;
            typedef map<string,string,KeyHasher,Allocator>           OptionDB;
            typedef increasing_comparator<string>                    StringComparator;
            typedef sorted_vector<string,StringComparator,Allocator> SortedStrings;
            typedef ordered_single<SortedStrings>                    OrderedStrings;

            class Options : public OptionDB
            {
            public:
                Options() throw();
                Options(const char   *);
                Options(const string &);

                virtual ~Options() throw();
                Options(const Options &);

                Options & parse( const string & );
                Options & parse( const char   * );

                Options & operator<<( const string & );
                Options & operator<<( const char   * );

                bool flag( const string &name ) const;

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

                static bool Flag(const Options *options, const string &name);
                static bool Flag(const Options *options, const char   *name);

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

                template <typename T> static inline
                T Get(const Options *options, const char *name, const T defaultValue=0)
                {
                    const string _(name);
                    return Get<T>(options,_,defaultValue);
                }

            private:
                Y_DISABLE_ASSIGN(Options);
            };

            


            class Format : public Object
            {
            public:
                const string name; //!< name for database of formats

                typedef intr_ptr<string,Format>                 Pointer;
                typedef set<string,Pointer,KeyHasher,Allocator> Set;

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

                const string &key() const throw();


            protected:
                explicit Format(const char *id, const char **ext); //!< setup
                OrderedStrings extensions;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Format);
                friend class Image;
            };


            static const at_exit::longevity life_time = 0;

            void use( Format *format );

            const Format & FormatFor(const string &filename) const;

            Bitmap *load(const string  &filename,
                         const size_t  depth,
                         RGBA2Data     &proc,
                         const Options *params) const;

            void    save(const string  &filename,
                         const Bitmap  &bmp,
                         Data2RGBA     &proc,
                         const Options *params) const;

            template <typename T>
            Bitmap *loadAs(const string &filename, const Options *params=NULL) const
            {
                PutRGBA<T> proc;
                return load(filename,sizeof(T),proc,params);
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

        };


    }
}

#endif

