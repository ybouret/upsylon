//! \file
#ifndef Y_CONCURRENT_SERVER_INCLUDED
#define Y_CONCURRENT_SERVER_INCLUDED 1

#include "y/concurrent/executor.hpp"
#include "y/functor.hpp"

namespace upsylon
{
    namespace concurrent
    {

        typedef functor<void,TL2(parallel&,lockable&)> job_type;
        typedef size_t                                 job_uuid;


        class server
        {
        public:
            class jnode
            {
            public:
                const job_uuid uuid;
                job_type       work;
                jnode         *next;
                jnode         *prev;
                jnode(const job_uuid u, const job_type &w ) :
                uuid(u), work(w), next(0), prev(0) {}
                ~jnode() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(jnode);
            };

            typedef core::list_of<jnode> jlist;
            typedef core::pool_of<jnode> jpool;

            virtual ~server() throw();

            

        protected:
            jlist    jobs;
            jpool    jmem;
            job_uuid uuid;

            explicit server() throw();
            
            job_uuid append( const job_type &J );
            void     remove( jnode          *j ) throw();
            void     remove_pending_jobs() throw();

            template <typename OBJECT_POINTER, typename METHOD_POINTER>
            job_uuid enqueue( OBJECT_POINTER host, METHOD_POINTER meth )
            {
                const job_type J(host,meth);
                return append(J);
            }

            template <typename FUNCTIONOID>
            job_uuid enqueue( const FUNCTIONOID &F )
            {
                const job_type J(F);
                return append(J);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(server);
        };

    }
}

#endif

