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
                const job_type work;
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

            job_uuid append( const job_type &J );


        protected:
            jlist    jobs;
            jpool    jmem;
            job_uuid uuid;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(server);
        };

    }
}

#endif

