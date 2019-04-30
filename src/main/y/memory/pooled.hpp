//! \file
#ifndef Y_MEMORY_POOLED_INCLUDED
#define Y_MEMORY_POOLED_INCLUDED 1

#include "y/memory/allocator.hpp"
#include "y/concurrent/singleton.hpp"
#include "y/type/ints.hpp"

namespace upsylon
{
    namespace memory
    {
        //! pooled memory allocator
        class pooled : public singleton<pooled>, public allocator
        {
        public:
            virtual void *acquire( size_t &n );                                            //!< allocator interface: acquire
            virtual void  release(void * &p, size_t &n) throw();                           //!< allocator interface: release
            bool          compact(void * &addr, size_t &capa, const size_t size ) throw(); //!< try to compact

            //! compact with high-level interface
            template <typename T>
            inline bool   compact_as( T * &addr, size_t &count, size_t &bytes, const size_t num ) throw()
            {
                assert(addr);
                assert(count);
                assert(bytes);
                assert(bytes>=count*sizeof(T));
                assert(num<=count);
                void  *p = static_cast<void *>(addr);
                size_t n = bytes;
                if( compact(p,n,num*sizeof(T)) )
                {
                    addr   = static_cast<T*>(p);
                    bytes  = n;
                    count  = bytes/sizeof(T);
                    return true;
                }
                else
                {
                    return false;
                }
                
            }
        private:
            explicit pooled() throw();
            virtual ~pooled() throw();

            friend class singleton<pooled>;
            
        public:
            //! come right before object::life_time
            static const at_exit::longevity life_time = limit_of<at_exit::longevity>::maximum - 2;
        };
    }
}

#endif

