//! \file
#ifndef Y_MEMORY_CBLOCK_INCLUDED
#define Y_MEMORY_CBLOCK_INCLUDED 1

#include "y/memory/buffer.hpp"


namespace upsylon
{

    namespace memory
    {
        //______________________________________________________________________
        //
        //
        //! lightweight local buffer
        //
        //______________________________________________________________________
        class cblock : public rw_buffer
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit cblock() throw();       //!< no memory
            explicit cblock(const size_t n); //!< setup with next_power_of_two
            virtual ~cblock() throw();       //!< cleanup

            //__________________________________________________________________
            //
            // rw_buffer interface
            //__________________________________________________________________
            virtual const void *ro()     const throw(); //!< block_addr
            virtual size_t      length() const throw(); //!< block_size

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            //! no-throw swap
            void swap_with( cblock &other ) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(cblock);
            const size_t block_size;
            const size_t block_exp2;
            void        *block_addr;
        };

       

    }

}

#endif
