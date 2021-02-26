
//! \file

#ifndef Y_CONCURRENT_TASK_PIPELINE_INCLUDED
#define Y_CONCURRENT_TASK_PIPELINE_INCLUDED 1

#include "y/concurrent/task/contracts.hpp"
#include "y/concurrent/task/worker.hpp"
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
            friend class worker;

            core::list_of_cpp<worker> crew;
            pending                   todo;
            settled                   done;
            size_t                    ready; //!< synchro counter

            virtual void call(const context &) throw();
            void         setup();
            void         finish() throw(); //!< broadcast and wait for end
            
            
        public:
            bool verbose; //!< from Y_VERBOSE_THREADS
            
        };
    }
}


#endif

