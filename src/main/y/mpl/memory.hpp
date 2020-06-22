//! \file

#ifndef Y_MP_MEMORY_INCLUDED
#define Y_MP_MEMORY_INCLUDED 1

#include "y/memory/allocator.hpp"
#include "y/concurrent/singleton.hpp"
#include "y/object.hpp"

namespace upsylon {
    
    namespace mpl {

        //======================================================================
        //
        //! dedicated memory manager
        //
        //======================================================================
        class manager : public singleton<manager>, public memory::allocator
        {
        public:
            virtual void *acquire( size_t &n );                    //!< allocator interface
            virtual void  release(void * &p, size_t &n ) throw();  //!< allocator interface
            uint8_t *   __acquire(size_t &n);                      //!< specialized acquire
            void        __release(uint8_t * &p,size_t &n) throw(); //!< specialized release
            
        private:
            explicit manager();
            virtual ~manager() throw();
            friend class singleton<manager>;
            
        public:
            static const at_exit::longevity life_time = object::life_time - 1; //!< need only objects
        };
    }
    
}


#endif

