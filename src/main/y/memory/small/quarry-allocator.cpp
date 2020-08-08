
#include "y/memory/small/quarry-allocator.hpp"
#include "y/memory/small/quarry.hpp"
#include "y/exceptions.hpp"
#include <cerrno>
#include <cstring>

namespace upsylon {

    namespace memory {

        namespace small {


            quarry_allocator:: ~quarry_allocator() throw()
            {
            }

            quarry_allocator:: quarry_allocator(lockable &usr_access,
                                                quarry   &usr_quarry) throw() :
            Access(usr_access),
            Quarry(usr_quarry)
            {
            }

            void *quarry_allocator:: fetch_locked(const size_t s)
            {
                Y_LOCK(Access);
                return Quarry[s].acquire();
            }


            void *quarry_allocator:: acquire(size_t &bytes,
                                             size_t &shift)
            {
                try
                {
                    if(bytes>vein::max_size)
                    {
                        throw libc::exception(EDOM,"quarry_allocator overflow");
                    }
                    size_t s = vein::min_exp2;
                    size_t n = vein::min_size; assert(size_t(1)<<s==n);
                    while(n<bytes)
                    {
                        ++s;
                        n <<= 1; assert(size_t(1)<<s==n);
                    }
                    void *p = fetch_locked(s);
                    bytes   = n;
                    shift   = s;
                    memset(p,0,bytes);
                    assert(size_t(1)<<shift==bytes);
                    return p;
                }
                catch(...)
                {
                    bytes = 0;
                    shift = 0;
                    throw;
                }
            }

            void  quarry_allocator:: release(void   * &addr,
                                             size_t   &bytes,
                                             size_t   &shift) throw()
            {
                assert(addr);
                assert(shift>=vein::min_exp2);
                assert(shift<=vein::max_exp2);
                assert(size_t(1)<<shift==bytes);
                Y_LOCK(Access);
                Quarry[shift].release(addr);
                bytes = 0;
                shift = 0;
                addr  = 0;
            }


            uint8_t * quarry_allocator:: acquire_bytes(size_t  &bytes, size_t &shift)
            {
                return static_cast<uint8_t*>( acquire(bytes,shift) );
            }

            void quarry_allocator:: release_bytes(uint8_t * &addr, size_t  &bytes, size_t &shift) throw()
            {
                release( *(void **)&addr, bytes, shift);
            }





        }
    }
}