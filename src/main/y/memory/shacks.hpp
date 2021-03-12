
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

        //! shared shacks
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

            //! make one object per slot
            template <typename T> inline
            void make()
            {
                slots_type &self = *this;
                for(size_t i=0;i<size();++i) (void) (*self[i]).make<T>();
            }

            //! make n objects per slot
            template <typename T> inline
            void make(const size_t n)
            {

                slots_type &self = *this;
                for(size_t i=0;i<size();++i) (void) (*self[i]).make<T>(n);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(shacks);
        };



    }

}


#endif

