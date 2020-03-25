//! \file

#ifndef Y_JARGON_TAGS_INCLUDED
#define Y_JARGON_TAGS_INCLUDED 1

#include "y/jargon/types.hpp"
#include "y/associative/suffix-tree.hpp"

namespace upsylon {

    namespace Jargon {
        
        typedef suffix_tree<Tag> TagsType; //!< base class for Tags
        
        //! global dictionary of shared tags
        class Tags : public singleton<Tags>, public TagsType
        {
        public:
            string * operator()(const string &s); //!< check string
            string * operator()(const char   *s); //!< check text
            string * operator()(const Tag     &); //!< check tag
            string * operator()(const char    C); //!< check single char
            
            //! return pointer to a registered string from any ID
            template <typename ID> static inline
            string * Make(const ID &id)
            {
                static Tags & tags = instance();
                return tags(id);
            }
            
            static void Display();          //!< display
            static void Release() throw();  //!< release resources
            
        private:
            static const at_exit::longevity life_time = 0;
            Y_DISABLE_COPY_AND_ASSIGN(Tags);
            explicit Tags();
            virtual ~Tags() throw();
            friend class singleton<Tags>;
            void display() const;

        };
        
    }
    
}

#endif

