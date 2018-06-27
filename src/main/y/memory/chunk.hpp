#ifndef YOCTO_MEMORY_CHUNK_INCLUDED
#define YOCTO_MEMORY_CHUNK_INCLUDED 1

#include "y/code/round.hpp"
#include "y/type/ints.hpp"
#include <iostream>

namespace upsylon
{
    namespace memory
    {

        //! a chunk of data to hold small memory blocks
        template <typename word_type>
        class chunk
        {
        public:
            //! maximum number of blocks that can be indexed
            static const size_t max_blocks = limit_of<word_type>::maximum;

            word_type      *data;            //!< data as array of word_type
            const size_t    words_increment; //!< one block_size per data[block_increment];
            word_type       first_available; //!< bookeeping
            word_type       still_available; //!< bookeeping
            const word_type provided_number; //!< initial count

            inline ~chunk() throw() {}

            //! compute parameters and format data
            /**
             \param block_size the desired block_size
             \param chunk_data user's provided memory data space
             \param chunk_size user's provided memory data length
             */
            inline  chunk(const size_t block_size,
                          void        *chunk_data,
                          const size_t chunk_size) throw() :
            data( static_cast<word_type *>(chunk_data) ),
            words_increment(0),
            first_available(0),
            still_available(0),
            provided_number(0)
            {
                assert(block_size>0);
                //______________________________________________________________
                //
                // use data to compute parameters
                //______________________________________________________________
                const size_t aligned_block_size = Y_ALIGN_FOR_ITEM(word_type,block_size);  // align the block size on the word_type
                (size_t &)   words_increment    = aligned_block_size/sizeof(word_type);    // how many words do we need to cover block_size
                const size_t blocks_in_data     = chunk_size/aligned_block_size;           // maximum unconstrained blocks in chunk_data
                (word_type &)provided_number    = still_available = static_cast<word_type>( (blocks_in_data<max_blocks) ? blocks_in_data : max_blocks );

                //______________________________________________________________
                //
                // format the chunk
                //______________________________________________________________
                word_type  *q = data;
                for(size_t  i = 0; i != still_available; q += words_increment )
                {
                    *q = word_type(++i);
                }
            }

            //! test if is empty
            inline bool is_empty() const throw() { return (still_available>=provided_number); }

            //! test if owned
            inline bool owns(const void *addr) const throw()
            {
                const word_type *p = (const word_type *)addr;
                return (p >= data) && (p < data+provided_number*words_increment);
            }

            //! acquire a zeroed block
            inline void *acquire() throw()
            {
                assert(still_available>0);
                assert(still_available<=provided_number);
                word_type     *p = &data[first_available*words_increment];
                first_available  = *p;

                --still_available;

                word_type *q = p;
                for(size_t i=words_increment;i>0;--i)
                {
                    *(q++) = 0;
                }
                return p;
            }

            //! release a previously acquired block
            inline void release(void *addr) throw()
            {
                assert(NULL!=addr);
                assert(still_available<provided_number);
                word_type *to_release = (word_type *)addr;
                assert( static_cast<ptrdiff_t>(to_release - data) % words_increment == 0 );

                // restore linking
                *to_release     = first_available;
                first_available = static_cast<word_type>( (to_release-data) / words_increment );

                assert( first_available == (to_release-data) / words_increment );

                // update status
                ++still_available;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(chunk);
        };

#if 0
        //! a chunk of data to hold small memory blocks
        template <typename T>
        class tChunk
        {
        public:
            typedef T            word_type;
            static  const size_t MaxBlocks = limit_of<word_type>::maximum;

            word_type          *data;            //!< first byte of data
            word_type           firstAvailable;  //!< bookeeping
            word_type           stillAvailable;  //!< bookeeping
            const size_t        blockIncrement;  //!< for memory access
            const word_type     providedNumber;  //!< initial count

            inline tChunk(void        *data_entry,
                          const size_t block_size,
                          const size_t chunk_size ) throw() :
            data( (word_type *)data_entry ),
            firstAvailable(0),
            stillAvailable(0),
            blockIncrement(0),
            providedNumber(0)
            {

                //______________________________________________________________
                //
                // compute all parameters
                //______________________________________________________________
                const size_t block_round    = Y_ALIGN_FOR_ITEM(word_type,block_size);
                (size_t    &)blockIncrement = (block_round/sizeof(word_type));

                const size_t top_blocks     = chunk_size/block_round;
                stillAvailable              = word_type( (MaxBlocks < top_blocks) ? MaxBlocks : top_blocks );
                (word_type &)providedNumber = stillAvailable;

                //______________________________________________________________
                //
                // format the chunk
                //______________________________________________________________
                word_type  *q = data;
                for(size_t  i = 0; i != stillAvailable; q += blockIncrement )
                {
                    *q = word_type(++i);
                }

            }

            inline ~tChunk() throw() {}

            //! acquire a zeroed block
            inline void *acquire() throw()
            {
                assert(stillAvailable>0);
                assert(stillAvailable<=providedNumber);
                word_type     *p = &data[firstAvailable*blockIncrement];
                firstAvailable   = *p;

                --stillAvailable;

                word_type *q = p;
                for(size_t i=blockIncrement;i>0;--i)
                {
                    *(q++) = 0;
                }
                return p;
            }


            //! release a previously allocated piece of memory
            inline void release(void *addr) throw()
            {
                assert(NULL!=addr);
                assert(stillAvailable<providedNumber);
                word_type *to_release = (word_type *)addr;
                assert( static_cast<ptrdiff_t>(to_release - data) % blockIncrement == 0 );

                // restore linking
                *to_release    = firstAvailable;
                firstAvailable = static_cast<word_type>( (to_release-data) / blockIncrement );

                assert( firstAvailable == (to_release-data) / blockIncrement );

                // update status
                ++stillAvailable;
            }

            //! to debug
            inline bool owns(const void *addr) const throw()
            {
                const word_type *p = (const word_type *)addr;
                return (p >= data) && (p < data+providedNumber*blockIncrement);
            }

            inline bool is_emtpty() const throw() { return providedNumber == stillAvailable; }



        private:
            Y_DISABLE_COPY_AND_ASSIGN(tChunk);
        };

        //! don't waste memory: use 16 bits to format
        typedef tChunk<uint16_t> kChunk;
#endif

    }
}

#endif

