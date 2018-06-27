#include "y/concurrent/barrier.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
	
	namespace concurrent
	{
		
		barrier:: barrier(size_t threshold_value) throw() :
		threshold( max_of<size_t>( threshold_value, 1 ) ),
		counter( threshold ),
		cycle(0),
		guard(),
		cv()
		{
		}
		
		barrier:: ~barrier() throw() {}
		
		bool barrier:: wait() throw()
		{
			bool status = false;
			guard.lock();
			{
				const size_t local_cycle = cycle;
				if( --counter <= 0 )
				{
					++cycle;
					counter = threshold;
					cv.broadcast();
					status = true;
				}
				else
				{
					while( local_cycle == cycle )
					{
						cv.wait( guard );
					}
					
				}
			}
			guard.unlock();
			return status;
		}
		
		
	}
	
}
