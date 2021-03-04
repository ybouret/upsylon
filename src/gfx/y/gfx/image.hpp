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

        struct image
        {

            class format : public entity
            {
            public:
                virtual ~format() throw();

                virtual bool matches(const string &ext) const = 0; //!< exact extension math

                template <typename FILENAME>
                bool handles(const FILENAME &filename) const
                {
                    const string ext = vfs::get_extension(filename);
                    return matches(ext);
                }

            protected:
                explicit format() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(format);
            };

            class named_format : public format
            {
            public:
                typedef intr_ptr<string,named_format> pointer;
                typedef hash_set<string,pointer>      database;

                const string       name;
                const string       extension_lowercase_regexp;
                const Jive::Motif  extension_compiled_pattern;
                virtual bool matches(const string &ext) const; //!< exact match

                virtual ~named_format() throw();

                const string & key() const throw(); //!< name

            protected:
                explicit named_format(const char *id, const char *rx);


            private:
                Y_DISABLE_COPY_AND_ASSIGN(named_format);
            };

            class io :
            public singleton<io>,
            public format,
            public named_format::database
            {
            public:

                void define( named_format *fmt );

                virtual bool matches(const string &ext) const;

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

