// \file
#ifndef Y_CONCURRENT_FAKE_LOCK
#define Y_CONCURRENT_FAKE_LOCK 1

#include "y/lockable.hpp"
#include "y/os/error.hpp"

namespace upsylon
{
    namespace concurrent
    {
        class fake_lock : public lockable
        {
        public:
            inline explicit fake_lock() throw() : count(0) {}
            inline virtual ~fake_lock() throw() { if(0!=count) { fatal_error("invalid ~fake_lock"); } }
            
            inline virtual void lock() throw() { ++count; }
            inline virtual void unlock() throw() { if(--count<0) { fatal_error("invalid fake unlock"); } }
            inline virtual bool try_lock() throw() { ++count; return true; }
            
        private:
            int count;
            Y_DISABLE_COPY_AND_ASSIGN(fake_lock);
        };
    }
}


#endif

