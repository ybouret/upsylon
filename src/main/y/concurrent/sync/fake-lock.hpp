//! \file
#ifndef Y_CONCURRENT_FAKE_LOCK
#define Y_CONCURRENT_FAKE_LOCK 1

#include "y/lockable.hpp"
#include "y/os/error.hpp"

namespace upsylon
{
    namespace concurrent
    {
        //! mimic a lockable behavior, single thread
        class fake_lock : public lockable
        {
        public:
            //! default constructor
            inline explicit fake_lock() throw() : count(0) {}
            
            //! default destructor, check consistency
            inline virtual ~fake_lock() throw() { if(0!=count) { fatal_error("invalid ~fake_lock"); } }
            
            //! increase count
            inline virtual void lock() throw() { ++count; }
            
            //! decrease count
            inline virtual void unlock() throw() { if(--count<0) { fatal_error("invalid fake unlock"); } }
            
            //! always true and increase count
            inline virtual bool try_lock() throw() { ++count; return true; }
            
        private:
            int count;
            Y_DISABLE_COPY_AND_ASSIGN(fake_lock);
        };
    }
}


#endif

