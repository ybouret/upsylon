
//! \file

#ifndef Y_CONCURRENT_TASK_PIPELINE_INCLUDED
#define Y_CONCURRENT_TASK_PIPELINE_INCLUDED 1

#include "y/concurrent/task/types.hpp"
#include "y/concurrent/executable.hpp"
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
        class pipeline : public executable, public supervisor
        {
        public:

            class task
            {
            public:
                task           *next;
                task           *prev;
                const job::uuid uuid;
                job::type       type;
                task(const job::uuid, const job::type &);
                ~task() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(task);
            };

            class drone :
            public authority<pipeline>,
            public condition,
            public thread
            {
            public:

                explicit drone(pipeline    &p,
                               const size_t user_size,
                               const size_t user_rank);

                virtual ~drone() throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(drone);
                static void stub(void*);
            };

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
            virtual job::uuid yield( const job::type & );
            virtual void      flush() throw();
            


        private:
            Y_DISABLE_COPY_AND_ASSIGN(pipeline);


            task  *query_task(const job::type &);
            void   store_task(task *) throw();
            void   remove_pending() throw();

            void   delete_pending() throw();
            void   delete_shallow() throw();

            virtual void call(const context &) throw();

            void         setup();

            core::list_of<task> pending; //!< pending tasks
            core::pool_of<task> shallow; //!< shallow tasks

            size_t ready;
            
        public:
            bool verbose; //!< from Y_VERBOSE_THREADS
            
        };
    }
}


#endif

