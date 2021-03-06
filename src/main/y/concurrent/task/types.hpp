//! \file

#ifndef Y_CONCURRENT_TASK_TYPE_INCLUDED
#define Y_CONCURRENT_TASK_TYPE_INCLUDED 1

#include "y/functor.hpp"
#include "y/container/sequence.hpp"
#include "y/concurrent/context.hpp"
#include "y/type/authority.hpp"

namespace upsylon
{
    namespace concurrent
    {

        //______________________________________________________________________
        //
        //! alias a context to provide info to jobs
        //______________________________________________________________________
        typedef authority<const context> anchor;

        //______________________________________________________________________
        //
        //
        //! job definitions
        /**
         the (anchored-)context of the calling thread is provided
         - for information
         - if some thread-specific data is to be accessed
         */
        //
        //______________________________________________________________________
        struct job
        {
            typedef TL2(const anchor &, lockable&) args;  //!< arguments for jobs
            typedef unsigned long                  uuid;  //!< for management
            typedef functor<void,args>             type;  //!< generic job
            typedef addressable<uuid>              uuids; //!< interface to uuids
            typedef accessible<type>               batch; //!< interface to types

            //! helper to precompile some jobs
            template <typename OBJECT, typename METHOD_POINTER> static inline
            void to(sequence<type> &jobs, OBJECT &host, METHOD_POINTER method)
            {
                const job::type J(&host,method);
                jobs.push_back(J);
            }


        };

        //______________________________________________________________________
        //
        //
        //! supervisor interface
        //
        //______________________________________________________________________
        class supervisor
        {
        public:
            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual job::uuid yield(const job::type &)                 = 0; //!< yield a new job
            virtual void      flush() throw()                          = 0; //!< flush current jobs
            virtual void      batch(job::uuids &, const job::batch &)  = 0; //!< process a batch of jobs
            virtual void      clear() throw()                          = 0; //!< remove pending
            virtual bool      completed(const job::uuid) throw()       = 0; //!< test if completed job

            //__________________________________________________________________
            //
            // helper
            //__________________________________________________________________

            //! wrapper
            template <typename OBJECT, typename METHOD_POINTER> inline
            job::uuid operator()(OBJECT &host, METHOD_POINTER method)
            {
                const job::type J(&host,method);
                return yield(J);
            }

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~supervisor() throw(); //!< cleanup

        protected:
            explicit  supervisor() throw();
            job::uuid jid; //!< shared job id to build uuids

        private:
            Y_DISABLE_COPY_AND_ASSIGN(supervisor);
        };

    }
}


#endif

