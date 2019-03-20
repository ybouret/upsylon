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

        class server
        {
        public:

            //------------------------------------------------------------------
            //
            // virtual interface
            //
            //------------------------------------------------------------------

            virtual ~server() throw();
            virtual job_uuid enqueue( const job_type &job ) = 0;
            virtual bool     is_done( const job_uuid  jid ) const throw() = 0;
            virtual bool     is_live( const job_uuid  jid ) const throw() = 0;

            //------------------------------------------------------------------
            //
            // non-virtual interface
            //
            //------------------------------------------------------------------
            template <typename OBJECT_POINTER, typename METHOD_POINTER>
            inline job_uuid enroll( OBJECT_POINTER host, METHOD_POINTER meth )
            {
                const job_type J(host,meth);
                return enqueue(J);
            }

        protected:
            job_uuid uuid;
            explicit server() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(server);
        };


        class sequential_server : public server
        {
        public:
            parallel  context;
            fake_lock access;

            explicit sequential_server() throw();
            virtual ~sequential_server() throw();

            virtual job_uuid enqueue( const job_type &job );
            virtual bool     is_done( const job_uuid      ) const throw(); //!< true
            virtual bool     is_live( const job_uuid      ) const throw(); //!< false

        private:
            Y_DISABLE_COPY_AND_ASSIGN(sequential_server);
        };
    }
    
}
#endif

