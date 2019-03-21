//! \file

#ifndef Y_CONCURRENT_DISPATCHER_INCLUDED
#define Y_CONCURRENT_DISPATCHER_INCLUDED 1

#include "y/concurrent/scheme/server.hpp"
#include "y/concurrent/threads.hpp"

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
                const int      valid;

                ~jnode() throw();                         //!< destructor
                jnode(const job_uuid, const job_type &J); //!< initialize

            private:
                Y_DISABLE_COPY_AND_ASSIGN(jnode);
            };

            typedef core::list_of<jnode> jlist;
            typedef core::pool_of<jnode> jpool_type;
            class jpool : public jpool_type
            {
            public:
                explicit jpool() throw() : jpool_type() {}

                virtual ~jpool() throw()
                {
                    clear();
                }

                jnode *fetch() throw()
                {
                    if(size)
                    {
                        return checked( query() );
                    }
                    else
                    {
                        return object::acquire1<jnode>();
                    }
                }

                void clear() throw()
                {
                    while(size)
                    {
                        jnode *j = checked( query() );
                        object::release1(j);
                    }
                }

                void gc() throw()
                {
                    for( jnode *j = top; j; j=checked(j)->next)
                        ;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(jpool);
                static jnode *checked( jnode *j ) throw() {
                    assert(j);
                    if(j->valid) { destruct(j); assert(0==j->valid); }
                    return j;
                }
            };


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
            jpool     junk;
            threads   workers;
        public:
            mutex    &access;
        private:
            bool      done;  //!< flag to quit loops
            size_t    ready; //!< for initial wait on cycle
            condition cycle;
            condition synch;
            
            static void start(void *, parallel &, lockable &);
            void loop( parallel &);


        public:
            bool &verbose;
        };

    }

}

#endif
