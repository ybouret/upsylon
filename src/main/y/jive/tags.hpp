
//! \file

#ifndef Y_JIVE_TAGS_INCLUDED
#define Y_JIVE_TAGS_INCLUDED 1

#include "y/jive/types.hpp"
#include "y/associative/suffix/tree.hpp"

namespace upsylon {

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! base type for Tags
        //
        //______________________________________________________________________
        typedef suffix_tree<Tag> TagsTree;

        //______________________________________________________________________
        //
        //
        //! database of reusable tags
        //
        //______________________________________________________________________
        class Tags : public singleton<Tags>, public TagsTree
        {
        public:
            const string * get(const char *buffer, const size_t buflen); //!< get/create
            const string * get(const char *buffer); //!< get/create
            const string * get(const char);         //!< get/create
            const string * get(const string &);     //!< get/create
            const string * get(const string *);     //!< get/create
            const string * get(const Tag    &);     //!< get/create

            //! generic helper
            template <typename T> static inline
            const string * Make(const T &id)
            {
                static Tags &tags = instance();
                return tags.get(id);
            }

            //! show content
            static void Display();

            //! helper to show
            const size_t maxLength;

        private:
            Y_SINGLETON_DECL(Tags);
            explicit Tags();
            virtual ~Tags() throw();
        };

    }

}

#endif
