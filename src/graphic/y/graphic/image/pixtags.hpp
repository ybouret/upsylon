//! \file

#ifndef Y_GRAPHIC_IMAGE_PIXTAGS_INCLUDED
#define Y_GRAPHIC_IMAGE_PIXTAGS_INCLUDED 1

#include "y/hashing/type-mark.hpp"
#include "y/string.hpp"
#include "y/graphic/color/rgb.hpp"
#include "y/graphic/types.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon {

    namespace Graphic {

        //! tag for a type of pixel (float,rgb,....)
        class PixTag_ : public Object
        {
        public:
            typedef hashing::type_mark_hasher<> Hasher; //!< special hasher


            const type_mark tid; //!< global type id
            const string    tag; //!< short tag

            const type_mark & key() const throw(); //!< for pointer
            virtual          ~PixTag_() throw(); //!< cleanup
            explicit          PixTag_(const type_spec &, const char *); //!< setup
        private:
            Y_DISABLE_COPY_AND_ASSIGN(PixTag_);
        };

        typedef intr_ptr<type_mark,PixTag_>           PixTag;   //!< alias
        typedef set<type_mark,PixTag,PixTag_::Hasher> PixTags_; //!< alias

        //! datbase of pixtags
        class PixTags : public PixTags_
        {
        public:
            virtual ~PixTags() throw(); //!< cleanup
            explicit PixTags();         //!< acquire memory

            //! create and insert a new type
            template <typename T> inline
            void create( const char *tag )
            {
                const PixTag p = new PixTag_( type_spec_of<T>(), tag );
                create(p);
            }

            //! query tag for a type
            template <typename T> inline
            const string &of() const
            {
                return of( type_spec_of<T>() );
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PixTags);
            void create( const PixTag & );
            const string &of(const type_spec &) const;
        };

    }
}

#endif

