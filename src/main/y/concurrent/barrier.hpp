//! \file

#ifndef Y_CONCURRENT_BARRIER_INCLUDED
#define Y_CONCURRENT_BARRIER_INCLUDED 1

#include "y/concurrent/condition.hpp"

namespace upsylon
{

	namespace concurrent
	{
		
		//! synchronization by barrier
		class barrier 
		{
		public:
			
			//! default constructor
			/**
				\param threshold_value release the barrier when that many threads are waiting
			 */
			explicit barrier( size_t threshold_value) throw();
			virtual ~barrier() throw();
            
			
			//! true if the last one to wait => everybody runs!
			bool wait() throw();
			
		private:
			Y_DISABLE_COPY_AND_ASSIGN(barrier);
			const size_t  threshold; /*!< count of threads to wait for         */
			size_t        counter;   /*!< how many threads are waiting so far  */
			size_t        cycle;     /*!< updated after each reached threshold */
			mutex         guard;     /*!< to protect access                    */
			condition     cv;        /*!< condition variable                   */
		};
		
	}
	
}

#endif
