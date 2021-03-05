
//! \file

#ifndef Y_GFX_IMAGE_NAMED_FORMAT_INCLUDED
#define Y_GFX_IMAGE_NAMED_FORMAT_INCLUDED 1

#include "y/gfx/image/format.hpp"
#include "y/ptr/intr.hpp"
#include "y/associative/hash/set.hpp"
#include "y/jive/pattern.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace image
        {
            class io; //!< forward declaration

            //__________________________________________________________________
            //
            //! named format (a.k.a JPEG, PNG, TIFF...)
            //__________________________________________________________________
            class named_format : public format
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef intr_ptr<string,named_format> pointer;  //!< alias
                typedef hash_set<string,pointer>      database; //!< alias

                //______________________________________________________________
                //
                // virtual interface
                //______________________________________________________________
                virtual bool matches(const string &ext) const;  //!< exact match
                virtual     ~named_format() throw();

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                const string & key() const throw(); //!< name for pointer

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const string       name;                        //!< vernacular
                const string       extension_lowercase_regexp;  //!< regular expression, lower case
                const Jive::Motif  extension_compiled_pattern;  //!< ignoring case

                
            protected:
                explicit named_format(const char *id, const char *rx); //!< setup


            private:
                Y_DISABLE_COPY_AND_ASSIGN(named_format);
                friend class io;
            };
        }

    }
}

#endif
