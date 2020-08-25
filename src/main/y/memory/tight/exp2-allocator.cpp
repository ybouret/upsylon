
#include "y/memory/tight/exp2-allocator.hpp"
#include "y/memory/tight/quarry.hpp"

namespace upsylon
{
    namespace memory
    {
        namespace tight
        {
            exp2_allocator:: ~exp2_allocator() throw()
            {
            }


            exp2_allocator :: exp2_allocator(lockable &s,
                                             quarry   &q) throw() :
            L(s),
            Q(q)
            {

            }

            void exp2_allocator:: optimize() throw()
            {
                Y_LOCK(L);
                Q.optimize();
            }
            
            const quarry &  exp2_allocator::operator*() const throw()
            {
                return Q;
            }


            uint8_t * exp2_allocator:: acquire_bytes(size_t  &bytes, size_t &shift)
            {
                return static_cast<uint8_t*>( acquire(bytes,shift) );
            }

            void exp2_allocator:: release_bytes(uint8_t * &addr, size_t  &bytes, size_t &shift) throw()
            {
                release( *(void **)&addr, bytes, shift);
            }



        }

    }

}
