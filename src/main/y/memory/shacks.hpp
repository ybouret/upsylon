
//! \file

#ifndef Y_MEMORY_SHACKS_INCLUDED
#define Y_MEMORY_SHACKS_INCLUDED 1

#include "y/memory/shack.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/sequence/slots.hpp"
#include "y/ptr/ref.hpp"



namespace upsylon {

    namespace memory
    {

        class shacks : public slots< ref_ptr<shack>, memory::dyadic >
        {
        public:
            typedef slots< ref_ptr<shack>, memory::dyadic > slots_type;
            enum construct_mode
            {
                construct_vacant,
                construct_filled
            };

            virtual ~shacks() throw();
            explicit shacks(const size_t n, const construct_mode = construct_vacant);

            void fill(); //!< fill with default shack

        private:
            Y_DISABLE_COPY_AND_ASSIGN(shacks);
        };



    }

}


#endif

