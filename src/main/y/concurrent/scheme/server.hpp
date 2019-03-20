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
        private:
            Y_DISABLE_COPY_AND_ASSIGN(server);
        };

    }
    
}
#endif

