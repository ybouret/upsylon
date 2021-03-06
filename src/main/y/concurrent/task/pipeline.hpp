
//! \file

#ifndef Y_CONCURRENT_TASK_PIPELINE_INCLUDED
#define Y_CONCURRENT_TASK_PIPELINE_INCLUDED 1

#include "y/concurrent/task/contracts.hpp"
#include "y/concurrent/task/worker.hpp"
#include "y/concurrent/component.hpp"
#include "y/concurrent/sync/condition.hpp"
#include "y/type/authority.hpp"
#include "y/collection.hpp"

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
		class pipeline : public component, public supervisor, public collection
		{
		public:
            //__________________________________________________________________
		    //
			// C++
			//__________________________________________________________________
			virtual ~pipeline() throw();               //!< cleanup
			explicit pipeline();                       //!< automatic setup
            explicit pipeline(Y_CONCURRENT_TOPO_ARGS); //!< manual setup

			//__________________________________________________________________
			//
			// interface : supervisor
			//__________________________________________________________________
			virtual job::uuid yield(const job::type &);
			virtual void      flush() throw();
            virtual void      batch(job::uuids &, const job::batch &);
            virtual void      clear() throw();
            virtual bool      completed(const job::uuid) throw();

            //__________________________________________________________________
            //
            // interface : collection
            //__________________________________________________________________
            virtual size_t    size() const throw(); //!< topo->size
            
            //__________________________________________________________________
            //
            // helper
            //__________________________________________________________________
            //! compute efficiency from speed_up and size()
            double efficiency(const double speed_up) const throw();
            
		private:
			Y_DISABLE_COPY_AND_ASSIGN(pipeline);
			friend class worker;
            condition         flushed; //!< to wait if necessary
            worker::list_type crew;    //!< available workers
			worker::list_type busy;    //!< running   workers
			pending           todo;    //!< tasks to do
			pending           proc;    //!< tasks in progress
			settled           done;    //!< done tasks, for re-use
			size_t            ready;   //!< synchro counter
            
            void      setup();                 //!< build all up to first sync
			void      finish()        throw(); //!< broadcast and wait for end
            void      loop(worker *)  throw(); //!< call within a thread

            void      deactivate_busy(worker *replica) throw(); //!< cancel deal, back to crew
            worker   *activate_worker()     throw();            //!< load head worker with head deal, NO broadcast/signal
            void      activate_by_primary() throw();            //!< activate and broadcast maximum of crew
            void      display_status() const;

		public:
			bool verbose; //!< from Y_VERBOSE_THREADS

		};
	}
}


#endif

