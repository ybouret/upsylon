
//! \file

#ifndef Y_JIVE_TAGS_INCLUDED
#define Y_JIVE_TAGS_INCLUDED 1

#include "y/jive/types.hpp"
#include "y/associative/suffix/tree.hpp"

namespace upsylon {

    namespace Jive
    {

        //! base type for Tags
        typedef suffix_tree<Tag> TagsTree;

        class Tags : public singleton<Tags>, public TagsTree
        {
        public:
            const string * get(const char *buffer, const size_t buflen );
            const string * get(const char *buffer);
            const string * get(const char);
            const string * get(const string &);
            const string * get(const string *);
            const string * get(const Tag    &);

            template <typename T> static inline
            const string * Make(const T &id)
            {
                static Tags &tags = instance();
                return tags.get(id);
            }

            static void Display();

            const size_t maxLength;

        private:
            Y_SINGLETON_DECL(Tags);
            explicit Tags();
            virtual ~Tags() throw();
        };

    }

}

#endif
