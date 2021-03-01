
//! \file

#ifndef Y_CONCURRENT_TASK_PIPELINE_INCLUDED
#define Y_CONCURRENT_TASK_PIPELINE_INCLUDED 1

#include "y/concurrent/task/contracts.hpp"
#include "y/concurrent/task/worker.hpp"
#include "y/concurrent/component.hpp"
#include "y/concurrent/sync/condition.hpp"
#include "y/type/authority.hpp"

//! macro helper
#define Y_PIPELINE_LN(MSG) do { if(verbose) { std::cerr << MSG << std::endl; } } while(false)


namespace upsylon
{
	namespace concurrent
	{

		//______________________________________________________________________
		//
		//
		//! pipeline of tasks
		//
		//______________________________________________________________________
		class pipeline : public component, public supervisor
		{
		public:
            static const unsigned DONE = 0x00;
            static const unsigned BUSY = 0x01;
            static const unsigned TODO = 0x02;
			
            //__________________________________________________________________
		    //
			// C++
			//__________________________________________________________________
			virtual ~pipeline() throw();  //!< cleanup
			explicit pipeline();          //!< setup threads and placement

			//__________________________________________________________________
			//
			// interface
			//__________________________________________________________________
			virtual job::uuid yield(const job::type &);
			virtual void      flush() throw();



		private:
			Y_DISABLE_COPY_AND_ASSIGN(pipeline);
			friend class worker;
            worker::list_type crew;  //!< available workers
			worker::list_type busy;  //!< running   workers
			pending           todo;  //!< tasks to do
			pending           proc;  //!< tasks in progress
			settled           done;  //!< done tasks, for re-use
			size_t            ready; //!< synchro counter
			bool              leave; //!< if this is the end...
            
            void      setup();                 //!< build all up to first sync
			void      finish()        throw(); //!< broadcast and wait for end
            void      loop(worker *)  throw(); //!< call within a thread
            unsigned  status()  const throw(); //!< get status, assuming locked

            void      deactivate_busy(worker *replica) throw(); //!< cancel deal, back to crew
            worker   *activate_worker() throw();                //!< load head worker with head deal, NO broadcast/signal

		public:
			bool verbose; //!< from Y_VERBOSE_THREADS

		};
	}
}


#endif

