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

        //! base class for server
        class server
        {
        public:
            //! node to hold jobs to do
            class jnode
            {
            public:
                const job_uuid uuid; //!< job ID
                job_type       work; //!< job content
                jnode         *next; //!< for list/pool
                jnode         *prev; //!< for list

                jnode(const job_uuid u, const job_type &w ); //!< constructor
                ~jnode() throw();                            //!< destructor

            private:
                Y_DISABLE_COPY_AND_ASSIGN(jnode);
            };

            typedef core::list_of<jnode> jlist; //!< list of active jobs
            typedef core::pool_of<jnode> jpool; //!< pool of dangling jobs

            virtual ~server() throw(); //!< desctructor

            

        protected:
            jlist    jobs; //!< jobs to do
            jpool    jmem; //!< memory bool
            job_uuid uuid; //!< internal uuid which increases for a new job

            explicit server() throw(); //!< initialize
            
            job_uuid append( const job_type &J );         //!< handle memory
            void     remove( jnode          *j ) throw(); //!< remove and store an active job
            void     remove_pending_jobs() throw();       //!< remove all pending jobs
            void     reserve(  size_t nj );               //!< reserve memory for jobs

            //! helper
            template <typename OBJECT_POINTER, typename METHOD_POINTER>
            job_uuid enqueue( OBJECT_POINTER host, METHOD_POINTER meth )
            {
                const job_type J(host,meth);
                return append(J);
            }

            //! helper
            template <typename FUNCTIONOID>
            job_uuid enqueue( const FUNCTIONOID &F )
            {
                const job_type J(F);
                return append(J);
            }

            //! look for uuid
            bool is_active( const job_uuid u ) const throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(server);
        };

    }

}


namespace upsylon
{
    namespace concurrent
    {
        //! sequential server
        class sequential_server : public server
        {
        public:
            concurrent::fake_lock access;  //!< mimic mutex
            parallel              context; //!< sequential info

            virtual ~sequential_server() throw();
            explicit sequential_server() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(sequential_server);
        };
    }
}

#endif

