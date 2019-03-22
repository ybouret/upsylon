//! \file
#ifndef Y_CONCURRENT_SERVER_INCLUDED
#define Y_CONCURRENT_SERVER_INCLUDED 1

#include "y/concurrent/executor.hpp"
#include "y/functor.hpp"

namespace upsylon
{
    namespace concurrent
    {

        typedef functor<void,TL2(parallel&,lockable&)> job_type; //!< a functor holds the job to do
        typedef size_t                                 job_uuid; //!< unique ID to look for task completion

        //! jobs server interface
        class server
        {
        public:
            virtual ~server() throw(); //!< destructor

            //------------------------------------------------------------------
            //
            // virtual interface
            //
            //------------------------------------------------------------------

            virtual job_uuid   enqueue( const job_type &job )               = 0; //!< enqueue a job
            virtual void       flush()   throw()                            = 0; //!< wait for all enqueued jobs to complete
            virtual executor & engine() throw()                             = 0; //!< get underlying engine

            //------------------------------------------------------------------
            //
            // non-virtual interface
            //
            //------------------------------------------------------------------
            //! wrapper
            template <typename OBJECT_POINTER, typename METHOD_POINTER>
            inline job_uuid enroll( OBJECT_POINTER host, METHOD_POINTER meth )
            {
                const job_type J(host,meth);
                return enqueue(J);
            }

            //! wrapper
            template <typename FUNCTIONOID>
            inline job_uuid enroll(const FUNCTIONOID &F )
            {
                const job_type J(F);
                return enqueue(J);
            }

        protected:
            job_uuid uuid; //!< uuid generator
            explicit server() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(server);
        };


        //! sequential server implementation
        class sequential_server : public server
        {
        public:
            explicit sequential_server() throw(); //!< initialize
            virtual ~sequential_server() throw(); //!< destructor

            virtual job_uuid   enqueue( const job_type &job );               //!< execute
            virtual void       flush() throw();                              //!< do nothing
            virtual executor & engine() throw();                             //!< implementation

        private:
            Y_DISABLE_COPY_AND_ASSIGN(sequential_server);
            sequential impl;
        };
    }
    
}
#endif

