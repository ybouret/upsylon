//! \file
#ifndef Y_MEMORY_SLAB_INCLUDED
#define Y_MEMORY_SLAB_INCLUDED 1

#include "y/dynamic.hpp"
#include "y/code/round.hpp"

namespace upsylon
{
    namespace memory
    {

        //______________________________________________________________________
        //
        //
        //! slab base class, NOT a POD!
        //
        //______________________________________________________________________
        class slab : public dynamic
        {
        public:
            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual ~slab() throw();                //!< cleanup
            virtual size_t size() const throw();    //!< dynamic interface: number of available memory object
            virtual size_t capacity() const throw();//!< dynamic interface: orignal number of memory objects

            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________
            void swap_with( slab &other ) throw(); //!< no-throw data swap

        protected:
            explicit slab(const size_t block_size,          //|
                          void        *chunk_data,          //|
                          const size_t chunk_size) throw(); //!< setup

            void *acquire_block();              //!< uses internal chunk
            void  release_block(void*) throw(); //!< uses internal chunk


        private:
            Y_DISABLE_COPY_AND_ASSIGN(slab);
            void      *impl;      //!< &data[0]
            void      *data[ 8 ]; //!< greater or equal to sizeof any __chunk
        };

        //______________________________________________________________________
        //
        //
        //! memory I/O for a fixed number of objects
        //
        //______________________________________________________________________
        template <typename T>
        class slab_of : public slab
        {
        public:
            static const size_t     block_size   = Y_ALIGN_FOR_ITEM(size_t,sizeof(T)); //!< aligned block size

            //! compute bytes to provide to hold num_objects
            static inline size_t bytes_for( const size_t num_objects ) throw()
            {
                return block_size * num_objects;
            }

            //! format underlying chunk
            inline slab_of(void *chunk_data,const size_t chunk_size) throw() :
            slab(block_size,chunk_data,chunk_size) {}


            inline virtual ~slab_of() throw() {}                                                   //!< destructor
            inline T       *acquire() throw()        {  return static_cast<T*>(acquire_block()); } //!< prepare memory area for sizeof(T)
            inline void     release(T *addr) throw() { release_block(addr); }                      //!<release a previously allocated block


        private:
            Y_DISABLE_COPY_AND_ASSIGN(slab_of);
        };


    }
}

#endif

