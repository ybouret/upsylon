
//! \file

#ifndef Y_CONCURRENT_TASK_PIPELINE_INCLUDED
#define Y_CONCURRENT_TASK_PIPELINE_INCLUDED 1

#include "y/concurrent/task/drone.hpp"
#include "y/concurrent/task/contracts.hpp"
#include "y/concurrent/executable.hpp"

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
            virtual void call(const context &) throw();

            core::list_of_cpp<drone> waiting;
            core::list_of_cpp<drone> running;
            executable::launcher     primary;
            contracts                cue;
            condition                activity;
            condition                flushed;
            size_t                   ready;
            bool                     halting;

            void setup();
            void cleanup() throw();
            void finish()  throw();
            void load1()   throw();
            void loadN()   throw();

            friend class drone;

        public:
            bool verbose; //!< from Y_VERBOSE_THREADS
            
        };
    }
}


#endif

