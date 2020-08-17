
//! \file
#ifndef Y_AP_NATURAL_INCLUDED
#define Y_AP_NATURAL_INCLUDED 1

#include "y/yap/number.hpp"
#include "y/memory/tight/quarry-allocator.hpp"

namespace upsylon {

    namespace yap {
        
        typedef memory::tight::quarry_allocator &memory_allocator; //!< alias

#define Y_APN_CTOR(MAX_BYTES) \
number(),\
bytes(0),\
words(0),\
count( words_for(MAX_BYTES) ),\
width(0),\
shift(0),\
word( acquire(count,width,shift) )


        //______________________________________________________________________
        //
        //
        //! natural drop-in unsigned replacement
        //
        //______________________________________________________________________
        class natural : public number
        {
        public:

            virtual ~natural() throw();

            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________
            static memory_allocator &instance();          //!< internal dedicated memory
            static memory_allocator &location() throw();  //!< internal dedicated memory
            static size_t            words_for(const size_t bytes) throw();


        private:
            size_t     bytes; //!< effective number of bytes
            size_t     words; //!< effective number of words = words_for(bytes)
            size_t     count; //!< memory words count
            size_t     width; //!< memory bytes count
            size_t     shift; //!< width = 1 << shift
            word_type *word;  //!< memory area

            static word_type *acquire(size_t &count, size_t &width, size_t &shift);
            static void       release(word_type *, size_t &count, size_t &width, size_t &shift) throw();
        };

    }

}

#endif
