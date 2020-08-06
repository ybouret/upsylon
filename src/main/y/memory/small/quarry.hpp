
//! \file

#ifndef Y_MEMORY_SMALL_QUARRY_INCLUDED
#define Y_MEMORY_SMALL_QUARRY_INCLUDED 1

#include "y/memory/small/stones.hpp"
#include "y/code/round.hpp"

namespace upsylon {

    namespace memory {

        namespace small {

            //__________________________________________________________________
            //
            //
            //! quarry of stones: handle (raw/dirty) power of two memory area
            //
            //__________________________________________________________________
            class quarry
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static const size_t   max_stones = stones::max_shift+1;          //!< alias
                static const size_t   wksp_bytes = max_stones * sizeof(stones);  //!< alias
                static const unsigned widths[64];                                //!< helper: output width in decimal for bytes

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                explicit quarry() throw(); //!< setup all stones
                virtual ~quarry() throw(); //!< cleanup all stones

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                void *pull(const size_t shift);                 //!< pull a memory area of 2^shift bytes
                void  push(void *, const size_t shift) throw(); //!< push a memory area of 2^shift bytes

                stones & operator[](const size_t shift)      throw(); //!< get the stones for a given shift
                stones & operator()(const size_t chunk_size) throw(); //!< self[ exp2_of(chunk_size) ]

                //______________________________________________________________
                //
                // helpers
                //______________________________________________________________
                static size_t         exp2_of( const size_t chunk_size ) throw(); //!< chunk_size = 1 << shift
                friend std::ostream & operator<<(std::ostream &, const quarry &); //!< display


            private:
                Y_DISABLE_COPY_AND_ASSIGN(quarry);
                stones       *ore;
            public:
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const size_t  undersized;  //!< number of undersized allocated bytes
                
            private:
                uint64_t      wksp[ Y_U64_FOR_SIZE(wksp_bytes) ];

            };

        }

    }

}

#endif
