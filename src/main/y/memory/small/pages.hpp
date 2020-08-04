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
            };

            class pages
            {
            public:
                typedef core::list_of<page> deposit_type;

                class deposit
                {
                public:
                    explicit deposit(const size_t usr_iln2) throw();
                    virtual ~deposit() throw();

                    const size_t page_iln2;
                    const size_t page_size;
                    size_t       bytes;      //!< size*page_size


                private:
                    Y_DISABLE_COPY_AND_ASSIGN(deposit);
                };

                typedef uint64_t            word_type;

                static const size_t one           = 1;
                static const size_t min_page_iln2 = ilog2_of<page>::value;
                static const size_t min_page_size = one << min_page_iln2;
                static const size_t full_deposits = sizeof(size_t)*8;

                static const size_t size_bytes = full_deposits * sizeof(size_t);
                static const size_t depo_bytes = full_deposits * sizeof(deposit);
                static const size_t wksp_bytes = size_bytes+depo_bytes;
                static const size_t wksp_align = Y_ALIGN_FOR_ITEM(word_type,wksp_bytes);
                static const size_t wksp_words = wksp_align/sizeof(word_type);

                const size_t max_page_size;
                const size_t max_page_iln2;
                const size_t deposit_count;
                
                pages(const size_t large_page_size);
                ~pages() throw();

                void * acquire(const size_t iln2);
                void   release(void *addr, const size_t iln2) throw();


            private:
                Y_DISABLE_COPY_AND_ASSIGN(pages);
                const size_t *size; //!< size[min_page_iln2..max_page_iln2]
                deposit      *depo; //!< depo[min_page_iln2..max_page_iln2];
                word_type     wksp[wksp_words];
            };


        }

    }

}

#endif
