//! \file

#ifndef Y_GRAPHIC_IMAGE_PIXTAGS_INCLUDED
#define Y_GRAPHIC_IMAGE_PIXTAGS_INCLUDED 1

#include "y/hashing/type-info.hpp"
#include "y/string.hpp"
#include "y/graphic/color/rgb.hpp"
#include "y/graphic/types.hpp"
#include "y/associative/set.hpp"
#include "y/ptr/intr.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon {

    namespace Graphic {

        class PixTag_ : public Object
        {
        public:
            typedef hashing::type_info_hasher<> Hasher;
            virtual ~PixTag_() throw();

            explicit PixTag_( const std::type_info &tid_, const char *tag_);

            const std::type_info &tid;
            const string          tag;

            const std::type_info & key() const throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PixTag_);
        };

        typedef intr_ptr<std::type_info,PixTag_>           PixTag;
        typedef set<std::type_info,PixTag,PixTag_::Hasher> PixTags_;

        class PixTags : public PixTags_
        {
        public:
            virtual ~PixTags() throw();
            explicit PixTags();

            template <typename T> inline
            void create( const char *tag )
            {
                const PixTag p = new PixTag_( typeid(T), tag );
                create(p);
            }

            template <typename T> inline
            const string &of() const
            {
                const std::type_info &tid = typeid(T);
                return of(tid);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(PixTags);
            void create( const PixTag & );
            const string &of(const std::type_info &) const;
        };

    }
}

#endif

