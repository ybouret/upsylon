//! \file

#ifndef Y_JARGON_TAGS_INCLUDED
#define Y_JARGON_TAGS_INCLUDED 1

#include "y/jargon/types.hpp"
#include "y/associative/suffix-tree.hpp"

namespace upsylon {

    namespace Jargon {
        
        typedef suffix_tree<Tag> TagsType;
        
        class Tags : public singleton<Tags>, public TagsType
        {
        public:
            string * operator()(const string &s);
            string * operator()(const char   *s);
            string * operator()(const Tag     &);
            
            template <typename ID> static inline
            string * Make(const ID &id)
            {
                static Tags & tags = instance();
                return tags(id);
            }
            
            void display() const;
            static void Display();
            
            static void Release() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Tags);
            explicit Tags();
            virtual ~Tags() throw();
            friend class singleton<Tags>;
        public:
            static const at_exit::longevity life_time = 0;
        };
        
    }
    
}

#endif

