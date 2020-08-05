
//! \file

#ifndef Y_MEMORY_SMALL_QUARRY_INCLUDED
#define Y_MEMORY_SMALL_QUARRY_INCLUDED 1

#include "y/memory/small/stones.hpp"
#include "y/code/round.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            //! quarry of stones: handle of the power of two area
            class quarry
            {
            public:
                static const size_t   max_stones = stones::max_shift+1;
                static const size_t   wksp_bytes = max_stones * sizeof(stones);
                static const unsigned widths[64]; //!< output width in decimal for bytes

                explicit quarry() throw();
                virtual ~quarry() throw();

                void *pull(const size_t shift);
                void  push(void *, const size_t shift) throw();

                stones & operator[](const size_t shift)      throw();
                stones & operator()(const size_t chunk_size) throw();

                friend std::ostream & operator<<(std::ostream &, const quarry &);

                static size_t exp2_of( const size_t chunk_size ) throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(quarry);
                stones       *ore;
            public:
                const size_t  undersized;  //!< number of undersized allocated bytes
                const size_t  acquired;    //!< total number of allocations
                const size_t  returned;    //!< total number of returned
                
            private:
                uint64_t      wksp[ Y_U64_FOR_SIZE(wksp_bytes) ];

            };

        }

    }

}

#endif
