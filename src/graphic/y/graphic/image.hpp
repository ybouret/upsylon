
//! \file

#ifndef Y_GRAPHIC_IMAGE_INCLUDED
#define Y_GRAPHIC_IMAGE_INCLUDED 1

#include "y/graphic/bitmap.hpp"
#include "y/graphic/color/rgba2data.hpp"
#include "y/graphic/color/data2rgba.hpp"
#include "y/string/convert.hpp"
#include "y/associative/map.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon {

    namespace Graphic {


        //! Images and Formats management
        class Image
        {
        public:

            typedef map<string,string,key_hasher<string,hashing::fnv>,memory::pooled> OptionDB;

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


            private:
                Y_DISABLE_ASSIGN(Options);
            };

            


            class Format : public Object
            {
            public:
                const string name; //!< name for database of formats

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

            protected:
                explicit Format(const char *id); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Format);
            };


        };


    }
}

#endif

