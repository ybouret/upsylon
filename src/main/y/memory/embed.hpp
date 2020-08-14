//! \file
#ifndef Y_MEMORY_EMBED_INCLUDED
#define Y_MEMORY_EMBED_INCLUDED 1

#include "y/memory/allocator.hpp"
#include "y/memory/marker.hpp"

namespace upsylon
{
    namespace memory
    {
        //! one acquisition for multiple data
        class embed
        {
        public:
            //! direct or indirect memory set
            enum policy_type
            {
                by_hook, //! pointer to address
                by_addr  //! just set address
            };

            //! cleanup
            ~embed() throw();

            //! prepare data by hook
            /**
             \param pp a pointer (not NULL) on a (NULL) address
             \param nb number of bytes
             */
            embed(void **pp, const size_t nb) throw();

            //! prepare data by addr
            embed(const size_t nb) throw();

            //! no throw copy for sequence
            embed(const embed &other ) throw();

            const marker        params; //!< initialized to (0,length)
            const policy_type   policy; //!< kind type
            union         
            {
                void *  addr;
                void ** hook;
            };

            //! acquire memory and dispatch chunks
            /**
             \param emb   an array of embedded info
             \param num   number of fields
             \param mem   memory allocator
             \param bytes allocated data
             \param data  effective space used by objects, *data <= bytes
             */
            static void *create(embed             *emb,
                                const size_t       num,
                                memory::allocator &mem,
                                size_t            &bytes,
                                size_t            *data = 0);

            
            //! named constructor
            template <typename T> inline
            static embed as(T * &addr, const size_t size) throw() { return embed((void**)&addr,size*sizeof(T)); }

        private:
            Y_DISABLE_ASSIGN(embed);
        };
    }
}

#endif

