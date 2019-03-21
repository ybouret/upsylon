//! \file

#ifndef Y_CONCURRENT_DISPATCHER_INCLUDED
#define Y_CONCURRENT_DISPATCHER_INCLUDED 1

#include "y/concurrent/scheme/server.hpp"
#include "y/concurrent/threads.hpp"

namespace upsylon
{
    namespace concurrent
    {
        //! MP jobs server
        class dispatcher : public server
        {
        public:
            //! internal node to hold a job
            class jnode
            {
            public:
                jnode         *next;  //!< for list/pool
                jnode         *prev;  //!< for list
                const job_uuid uuid;  //!< given by dispatcher
                job_type       call;  //!< the job to do
                const int      valid; //!< flag to manage memory

                ~jnode() throw();                         //!< destructor
                jnode(const job_uuid, const job_type &J); //!< initialize

            private:
                Y_DISABLE_COPY_AND_ASSIGN(jnode);
            };

            typedef core::list_of<jnode> jlist;      //!< alias
            typedef core::pool_of<jnode> jpool_type; //!< alias

            //! pool to manage valid and invalid nodes
            class jpool : public jpool_type
            {
            public:
                explicit jpool() throw(); //!< initialize
                virtual ~jpool() throw(); //!< destructor
                jnode   *fetch();         //!< fetch a not valid done
                void     clear() throw(); //!< delete all nodes
                void     gc()    throw(); //!< invalidate all nodes

            private:
                Y_DISABLE_COPY_AND_ASSIGN(jpool);
                static jnode *checked( jnode *j ) throw();
            };


            explicit dispatcher( const bool v = false ); //!< initialize
            virtual ~dispatcher() throw();               //!< destruct, all pending jobs are removed

            void remove_pending() throw(); //!< remove all pending jobs
            void reserve_jobs( size_t n ); //!< memory to reserve for some jobs


            virtual bool       is_done( const job_uuid  jid ) const throw(); //!< look up
            virtual bool       is_live( const job_uuid  jid ) const throw(); //!< loop up
            virtual job_uuid   enqueue( const job_type &job );  //!< enqueue a job
            virtual void       join() throw();
            virtual executor & engine() throw();  //!< implementation
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(dispatcher);
            jlist     jobs;
            jpool     junk;
            jpool     jerr;
            threads   workers;
        public:
            mutex    &access; //!< shared mutex
        private:
            bool      done;  //!< flag to quit loops
            size_t    ready; //!< for initial wait on cycle
            condition cycle;
            condition synch;
            
            static void start(void *, parallel &, lockable &);
            void loop( parallel &);


        public:
            bool &verbose; //!< workers.verbose
        };

    }

}

#endif
