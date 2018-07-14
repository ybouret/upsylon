//! \file
#ifndef Y_HASH_TABLE_INCLUDED
#define Y_HASH_TABLE_INCLUDED 1

#include "y/core/addr-list.hpp"
#include "y/memory/slab.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{

    namespace core
    {
        class hash_table_info
        {
        public:
            inline virtual ~hash_table_info() throw() {}
            inline explicit hash_table_info() throw() {}
            static const size_t load_factor = 4;
            static const size_t min_slots   = 8;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(hash_table_info);
        };

        template <typename NODE>
        class hash_table : public hash_table_info
        {
        public:
            typedef core::list_of<NODE> slot_type;
            typedef memory::slab<NODE>  slab_type;

            inline  hash_table() throw() : slot(0), slots(0), smask(0), cache(0,0), buffer(0), allocated(0)
            {
            }

            inline  hash_table(const size_t n) throw() : slot(0), slots(0), smask(0), cache(0,0), buffer(0), allocated(0)
            {
                if(n>0)
                {
                    const size_t nreq       = n;
                    const size_t num_slots  = next_power_of_two(max_of<size_t>(nreq/load_factor,min_slots));
                    const size_t slot_offset = 0;
                    const size_t slot_length = num_slots * sizeof(slot_type);
                    const size_t slab_offset = memory::align(slot_offset+slot_length);
                    const size_t slab_length = slab_type::bytes_for(nreq);

                    allocated = memory::align(slab_offset+slab_length);
                }
            }


            inline  virtual ~hash_table() throw()
            {
            }

            slot_type *slot;  //!< slots entry
            size_t     slots; //!< 0 or a power of two
            size_t     smask; //!< slots-1
            slab_type  cache; //!< to store nodes

        private:
            Y_DISABLE_COPY_AND_ASSIGN(hash_table);
            void   *buffer;
            size_t  allocated;
        };
    }

}

#endif

