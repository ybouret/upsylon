#include "y/container/key-address.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"

namespace upsylon {

    namespace core {

        static inline ptrdiff_t __compare_addr(const void *lhs, const void *rhs) throw()
        {
            const char *l = static_cast<const char *>(lhs);
            const char *r = static_cast<const char *>(rhs);
            return  static_cast<ptrdiff_t>(l-r);
        }
        
        void sort_addresses( void **addr, const size_t n) throw()
        {
            assert(addr);
            lightweight_array<void*> arr( addr, n );
            hsort(arr,__compare_addr);
        }

        int  lcmp_addresses( void * const *lhs, void * const *rhs, const size_t n) throw()
        {
            return comparison::increasing_lexicographic<void*>(lhs, rhs, n);
        }

    }

}

