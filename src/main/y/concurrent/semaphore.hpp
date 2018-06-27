//! \file

#ifndef Y_CONCURRENT_SEMAPHORE_INCLUDED
#define Y_CONCURRENT_SEMAPHORE_INCLUDED 1

#include "y/concurrent/condition.hpp"


namespace upsylon {
	
	namespace concurrent
	{
        //! portable semaphore class
		class semaphore
        {
		public:
            //! construct semaphore with initialPermits
			explicit semaphore( const uint32_t initialPermits ) throw();

            //! destructor
			virtual ~semaphore() throw();

            //! wait for a permit
			void wait() throw();

            //! permit is available
			void post() throw();
			
		private:
			Y_DISABLE_COPY_AND_ASSIGN(semaphore);
			uint32_t  count_;
			uint32_t  waiters_count_;
			mutex     count_lock_;
			condition count_nonzero_;
		};
	}
}

#endif
