//! \file

#ifndef Y_CONCURRENT_NEXUS_INCLUDED
#define Y_CONCURRENT_NEXUS_INCLUDED 1

#include "y/concurrent/scheme/server.hpp"
#include "y/concurrent/threads.hpp"
#include "y/core/list.hpp"
#include "y/core/pool.hpp"

namespace upsylon {

    namespace concurrent {

        //----------------------------------------------------------------------
        //
        //! a nexus is a parallel job server
        //
        //----------------------------------------------------------------------
        class nexus : public server
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            class jpool; //!< forward declaration for friendship

            //__________________________________________________________________
            //
            //! reusable internal node to hold a job
            //__________________________________________________________________
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
                jnode *destruct() throw();
                friend class jpool;
                friend class nexus;
            };

            typedef core::list_of<jnode> jlist;      //!< alias
            typedef core::pool_of<jnode> jpool_type; //!< alias

            //__________________________________________________________________
            //
            //! pool to manage valid and invalid nodes
            //__________________________________________________________________
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

            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            explicit nexus(const bool v= false);    //!< initialize
            virtual ~nexus() throw();               //!< destruct, all pending jobs are removed

            //------------------------------------------------------------------
            //
            // virtual interface
            //
            //------------------------------------------------------------------
            virtual job_uuid   enqueue(const job_type &);                     //!< enqueue a job
            virtual void       flush()  throw();                              //!< wait for all enqueued jobs
            virtual executor & engine() throw();                              //!< implementation
            virtual void       process(job_uuids &uuids, const job_batch &);  //!< batch

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            void   remove_pending() throw();                 //!< remove all pending jobs
            void   reserve_jobs(size_t);                     //!< memory to reserve for some jobs
            double efficiency(const double speed_up) const;  //!< compute efficiency from threads

        private:
            Y_DISABLE_COPY_AND_ASSIGN(nexus);
            jlist     pending; //!< pending jobs
            jlist     current; //!< currently processed jobs
            jpool     aborted; //!< jobs that raised an exception
            jpool     storage; //!< main thread memory
            threads   workers; //!< the engine
            
        public:
            mutex    &access; //!< shared mutex

        private:
            size_t    prepared;  //!< for first synchronization
            bool      stopping;  //!< flag to quit loops
            condition activity;
            condition flushing;
            
            static void start(void *, parallel &, lockable &);
            void loop( parallel &);


        public:
            bool &verbose; //!< workers.verbose
        };

    }

}

#endif
