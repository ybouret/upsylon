//! \file
#ifndef YOCTO_MEMORY_CHUNK_INCLUDED
#define YOCTO_MEMORY_CHUNK_INCLUDED 1

#include "y/code/round.hpp"
#include "y/type/ints.hpp"
#include "y/memory/io.hpp"
#include "y/memory/ownership.hpp"

namespace upsylon
{
    namespace memory
    {

        //! a chunk of data to hold small memory blocks
        template <typename word_type>
        class __chunk
        {
        public:
            //! word size for internal computations
            static const size_t word_size = sizeof(word_type);
            
           

            //! maximum number of blocks that can be indexed
            static const size_t max_blocks = limit_of<word_type>::maximum;
            __chunk         *next;            //!< for list
            __chunk         *prev;            //!< for list
            word_type       *data;            //!< data as array of word_type
            const word_type *last;            //!< keep track of chunk size
            const size_t     words_increment; //!< one block_size per data[block_increment];
            word_type        first_available; //!< bookeeping
            word_type        still_available; //!< bookeeping
            const word_type  provided_number; //!< initial count

            //! destructor
            inline __chunk() throw(){}
            
            //! compute parameters and format data
            /**
             \param block_size the desired block_size
             \param chunk_data user's provided memory data space
             \param chunk_size user's provided memory data length
             */
            inline  __chunk(const size_t block_size,
                            void        *chunk_data,
                            const size_t chunk_size) throw() :
            next(0),
            prev(0),
            data( io::cast<word_type>(chunk_data,0) ),
            last( io::cast<word_type>(chunk_data,chunk_size) ),
            words_increment(0),
            first_available(0),
            still_available(0),
            provided_number(0)
            {
                assert( block_size >= sizeof(word_type)  );
                assert( chunk_size == size_t(memory::io::delta(data,last)) );
                assert( chunk_size<=0 || chunk_size >= sizeof(word_type)  );
                assert( chunk_size<=0 || chunk_size >= sizeof(block_size) );
                
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
                for(size_t  i = 0; i != provided_number; q += words_increment )
                {
                    *q = word_type(++i);
                }
            }

            //! test if is empty
            inline bool   is_empty()  const throw() { return (still_available>=provided_number); }

            //! for info
            inline size_t allocated() const throw() { return provided_number-still_available;    }

            //! test if address is owned by this chunk
            inline bool owns(const void *addr) const throw()
            {
                const word_type *p = (const word_type *)addr;
                return (p >= data) && (p<last);
            }

            //! get ownership for look up
            inline ownership whose(const void *addr) const throw()
            {
                const word_type *p = (const word_type *)addr;
                if(p<data)
                {
                    return owned_by_prev;
                }
                else
                {
                    if(p>=last)
                    {
                        return owned_by_next;
                    }
                    else
                    {
                        return owned_by_this;
                    }
                }
            }

            //! acquire a zeroed block
            inline void *acquire() throw()
            {
                assert(still_available>0);
                assert(still_available<=provided_number);

                // update status
                word_type     *p = &data[ size_t(first_available)*words_increment];
                first_available  = *p;
                --still_available;

                // zero the block
                word_type     *q = p;
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
            Y_DISABLE_COPY_AND_ASSIGN(__chunk);
        };

        template <typename T> const size_t __chunk<T>::max_blocks;

    }
}

#endif

