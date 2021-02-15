//! \file

#ifndef Y_CONCURRENT_CONDITION_INCLUDED
#define Y_CONCURRENT_CONDITION_INCLUDED 1


#include "y/concurrent/mutex.hpp"

namespace upsylon
{

#if defined(Y_WIN)
	namespace win32
	{	
		class semaphore
		{
		public:
			semaphore() throw();
			~semaphore() throw();
			void wait() throw();
			void post() throw();
		private:
			HANDLE sem_;
			Y_DISABLE_COPY_AND_ASSIGN(semaphore);
		};
	}
#endif

	namespace concurrent
	{
		
		//! portable condition variable
		class condition 
		{
		public:
			explicit condition() throw();
			virtual ~condition() throw();
			
			//! wait on a LOCKED mutex, returns on  the LOCKED mutex
			void     wait( mutex & ) throw();
			
			//! allow one blocked thread to wake up
			void     signal()    throw();
			
			//! allow the blocked threads to wake up
			void     broadcast() throw();
			
		private:
			Y_DISABLE_COPY_AND_ASSIGN(condition);
            
#if defined(Y_BSD)
			pthread_cond_t cond; //!< using native condition
#endif

#if defined(Y_WIN)
			mutex            cv_lock;      /*!< condition lock               */
			int              cv_waiting;   /*!< waiting count                */
			int              cv_signals;   /*!< signals count                */
			win32::semaphore cv_wait_sem;  /*!< Win32 semaphore when waiting */
			win32::semaphore cv_done_sem;  /*!< Win32 semaphore when done    */
#endif

		};
	}

}

#endif
