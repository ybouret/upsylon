//! \file

#ifndef Y_MEMORY_SMALL_PAGES_INCLUDED
#define Y_MEMORY_SMALL_PAGES_INCLUDED 1

#include "y/code/ilog2.hpp"
#include "y/core/list.hpp"
#include "y/code/round.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            struct page
            {
                page *next;
                page *prev;
                typedef core::list_of<page> list;
            };

            class pages
            {
            public:
                class page_list : public page::list
                {
                public:
                    explicit page_list(size_t iln2) throw();
                    virtual ~page_list() throw();
                    
                    const size_t iln2;
                    
                private:
                    Y_DISABLE_COPY_AND_ASSIGN(page_list);
                };
                
                typedef      uint32_t word_type;
                
                static const size_t   one       = 1;
                static const unsigned min_iln2  = ilog2_of<page>::value;
                static const size_t   min_size  = one << min_iln2;
                static const unsigned max_iln2  = (sizeof(word_type)<<3)-1;
                static const size_t   max_size  = one << max_iln2;
                static const size_t   required  = (max_iln2+1) * sizeof(page_list);
                static const size_t   _aligned  = Y_ALIGN_FOR_ITEM(word_type,required);
                static const size_t   in_words  = _aligned/sizeof(word_type);

                explicit pages(const size_t large_size);
                virtual ~pages() throw();
                
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(pages);
                page_list *plist;
                word_type  wksp[in_words];
            };


        }

    }

}

#endif
