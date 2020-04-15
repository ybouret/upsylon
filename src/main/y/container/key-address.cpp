#include "y/container/key-address.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"
#include "y/sequence/array.hpp"
#include "y/os/endian.hpp"

namespace upsylon {

    namespace core {

        static inline ptrdiff_t __compare_addr(const void *lhs, const void *rhs) throw()
        {
            const char *l = static_cast<const char *>(lhs);
            const char *r = static_cast<const char *>(rhs);
            return  static_cast<ptrdiff_t>(l-r);
        }
        
        void addresses::sort( void **addr, const size_t n) throw()
        {
            assert(addr);
            lightweight_array<void*> arr( addr, n );
            hsort(arr,__compare_addr);
        }

        int  addresses::lcmp( void * const *lhs, void * const *rhs, const size_t n) throw()
        {
            return comparison::increasing_lexicographic<void*>(lhs, rhs, n);
        }
        
    }

}

