//! \file

#ifndef Y_GFX_IMAGE_INCLUDED
#define Y_GFX_IMAGE_INCLUDED 1

#include "y/gfx/pixmap.hpp"
#include "y/gfx/color/type-to-rgba.hpp"
#include "y/gfx/color/rgba-to-type.hpp"
#include "y/jive/pattern.hpp"
#include "y/fs/vfs.hpp"
#include "y/ptr/intr.hpp"
#include "y/associative/hash/set.hpp"

namespace upsylon
{
    namespace graphic
    {

        //! image architecture
        struct image
        {

            //! format object: match/load/save
            class format : public entity
            {
            public:

                virtual     ~format() throw();                     //!< cleanup
                virtual bool matches(const string &ext) const = 0; //!< exact extension math

                //! load a new bitmap
                virtual bitmap load(const string       &file,
                                    const void         *opts,
                                    const rgba_to_type &conv) = 0;

                //! handles file if matches extension
                template <typename FILENAME>
                bool handles(const FILENAME &filename) const
                {
                    const string ext = vfs::get_extension(filename);
                    return matches(ext);
                }

            protected:
                explicit format() throw(); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(format);
            };

            //! named format (a.k.a JPEG, PNG, TIFF...)
            class named_format : public format
            {
            public:
                typedef intr_ptr<string,named_format> pointer;  //!< alias
                typedef hash_set<string,pointer>      database; //!< alias

                const string       name;                        //!< vernacular
                const string       extension_lowercase_regexp;  //!< regular expression, lower case
                const Jive::Motif  extension_compiled_pattern;  //!< ignoring case

                virtual bool matches(const string &ext) const;  //!< exact match
                virtual     ~named_format() throw();


                const string & key() const throw(); //!< name for pointer

            protected:
                explicit named_format(const char *id, const char *rx); //!< setup


            private:
                Y_DISABLE_COPY_AND_ASSIGN(named_format);
            };

            //! compound object to handle multiple named formats
            class io :
            public singleton<io>,
            public format,
            public named_format::database
            {
            public:

                void define( named_format *fmt ); //!< insert, taken care of
                void standard();                  //!< insert undefined standard

                virtual bool   matches(const string &ext) const;
                virtual bitmap load(const string       &file,
                                    const void         *opts,
                                    const rgba_to_type &conv);
            private:
                Y_DISABLE_COPY_AND_ASSIGN(io);
                explicit io();
                virtual ~io() throw();
                Y_SINGLETON_DECL_WITH(0,io);
            };
        };


    }
}

#endif

