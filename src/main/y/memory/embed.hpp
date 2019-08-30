//! \file
#ifndef Y_MEMORY_EMBED_INCLUDED
#define Y_MEMORY_EMBED_INCLUDED 1

#include "y/memory/allocator.hpp"

namespace upsylon
{
    namespace memory
    {
        //! one acquisition for multiple data
        class embed
        {
        public:
            //! cleanup
            ~embed() throw();

            //! prepare data
            /**
             \param pp a pointer (not NULL) on a (NULL) address
             \param nb number of bytes
             */
            embed(void **pp, const size_t nb) throw();

            //! no throw copy for sequence
            embed(const embed &other ) throw();

            const size_t length; //!< user's required bytes
            const size_t offset; //!< initialized to 0, then computed
            void       **ppHead; //!< pointer to address

            //! acquire memory and dispatch chunks
            static void *create(embed             *emb,
                                const size_t       num,
                                memory::allocator &mem,
                                size_t            &bytes);

            //! use global memory
            static void *create_global(embed *emb, const size_t num, size_t &bytes);

            //! named constructor
            template <typename T>
            static embed as(T * &addr, const size_t size) throw() { return embed((void**)&addr,size*sizeof(T)); }

        private:
            Y_DISABLE_ASSIGN(embed);
        };

    }
}

#endif

