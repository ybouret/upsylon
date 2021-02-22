
//! \file

#ifndef Y_CONCURRENT_TASK_PIPELINE_INCLUDED
#define Y_CONCURRENT_TASK_PIPELINE_INCLUDED 1

#include "y/concurrent/task/types.hpp"
#include "y/concurrent/executable.hpp"
#include "y/concurrent/thread.hpp"
#include "y/concurrent/sync/condition.hpp"
#include "y/core/inode.hpp"

//! VERBOSITY
#define Y_VERBOSE_PIPELINE "Y_VERBOSE_PIPELINE"

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
            //__________________________________________________________________
            //
            // types and definition
            //___________________________________________________________________

            //! dynamic engine = thread
            class engine : public object, public executable::launcher, public inode<engine>
            {
            public:
                virtual ~engine() throw();                  //!< cleanup
                explicit engine(executable     &user_host,  //|
                                const size_t    user_size,  //|
                                const size_t    user_rank); //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(engine);
            };

            //! list of engines, alias
            typedef core::list_of_cpp<engine> engines;


            //! task = job wrapper
            class task
            {
            public:
                task           *next; //!< for list
                task           *prev; //!< for list
                const job::uuid uuid; //!< from supervisor
                job::type       code; //!< to execute
                size_t          priv; //!< private...

                task(const job::uuid U, const job::type &J); //!< setup
                ~task() throw();                             //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(task);
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
            void    trim() throw(); //!< remove shallow tasks
            void    free() throw(); //!< remove pending tasks
            
            virtual job::uuid enqueue( const job::type & );


        private:
            Y_DISABLE_COPY_AND_ASSIGN(pipeline);
            virtual void call(const context &) throw();

            engines             waiting;
            engines             working;
            core::list_of<task> pending;
            core::list_of<task> shallow;
            size_t              ready;
            condition           start;
            bool                built;
            
            void setup();
            void cleanup() throw();



            task * create_task(const job::type &J);

            
        public:
            bool verbose; //!< from Y_VERBOSE_PIPELINE
            
        };
    }
}


#endif

