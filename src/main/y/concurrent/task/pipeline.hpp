
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

            virtual ~pipeline() throw();  //!< cleanup
            explicit pipeline();          //!< setup threads and placement

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pipeline);
            virtual void call(const context &) throw();

            engines   waiting;
            engines   working;
            size_t    ready;
            condition start;

            bool    built;
            
            void setup();
            void cleanup() throw();
            
        public:
            bool verbose; //!< from Y_VERBOSE_PIPELINE
            
        };
    }
}


#endif

