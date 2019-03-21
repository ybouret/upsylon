//! \file

#ifndef Y_CONCURRENT_DISPATCHER_INCLUDED
#define Y_CONCURRENT_DISPATCHER_INCLUDED 1

#include "y/concurrent/scheme/server.hpp"
#include "y/concurrent/threads.hpp"
#include "y/memory/object-pool.hpp"

namespace upsylon
{
    namespace concurrent
    {

        class dispatcher : public server
        {
        public:
            //! internal node
            class jnode
            {
            public:
                jnode         *next;
                jnode         *prev;
                const job_uuid uuid;
                job_type       call;

                ~jnode() throw();                         //!< destructor
                jnode(const job_uuid, const job_type &J); //!< initialize

            private:
                Y_DISABLE_COPY_AND_ASSIGN(jnode);
            };

            typedef core::list_of<jnode>       jlist;
            typedef memory::object_pool<jnode> jpool;

            explicit dispatcher( const bool v = false );
            virtual ~dispatcher() throw();

            void remove_pending() throw();
            void reserve_jobs( size_t n );

            virtual bool       is_done( const job_uuid  jid ) const throw();
            virtual bool       is_live( const job_uuid  jid ) const throw();
            virtual job_uuid   enqueue( const job_type &job );
            virtual void       join() throw();
            virtual executor & engine() throw();  //!< implementation

        private:
            Y_DISABLE_COPY_AND_ASSIGN(dispatcher);
            jlist     jobs;
            jpool     jmem;
            threads   workers;
            bool      done; //!< flag to quit loops

            static void start(void *, parallel &, lockable &);
            void loop( parallel &, lockable & );


        public:
            bool &verbose;
        };

    }

}

#endif
