//! \file

#ifndef Y_CONCURRENT_TASK_TYPE_INCLUDED
#define Y_CONCURRENT_TASK_TYPE_INCLUDED 1

#include "y/functor.hpp"
#include "y/sequence/addressable.hpp"
#include "y/container/sequence.hpp"

namespace upsylon
{
    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        //! job definitions
        //
        //______________________________________________________________________
        struct job
        {
            typedef unsigned long                uuid;  //!< for management
            typedef functor<void,TL1(lockable&)> type;  //!< generic job
            typedef addressable<uuid>            uuids; //!< interface to uuids
            typedef accessible<type>             batch; //!< interface to types

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
            virtual job::uuid yield(const job::type &)                = 0; //!< yield a new job
            virtual void      flush() throw()                         = 0; //!< flush current jobs
            virtual void      batch(job::uuids &, const job::batch &) = 0; //!< process a batch of jobs
            virtual void      clear() throw()                         = 0; //!< remove pending

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

