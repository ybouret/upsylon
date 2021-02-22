//! \file

#ifndef Y_CONCURRENT_TASK_TYPE_INCLUDED
#define Y_CONCURRENT_TASK_TYPE_INCLUDED 1

#include "y/functor.hpp"

namespace upsylon
{
    namespace concurrent
    {

        //! job definitions
        struct job
        {
            typedef unsigned long                uuid; //!< for management
            typedef functor<void,TL1(lockable&)> type; //!< generic job
        };

        //! supervisor interface
        class supervisor
        {
        public:
            virtual ~supervisor() throw(); //!< cleanup

            //! job->uuid
            virtual job::uuid yield(const job::type &) = 0;
            virtual void      flush() throw()          = 0;

            //! wrapper
            template <typename OBJECT, typename METHOD_POINTER> inline
            job::uuid operator()(OBJECT &host, METHOD_POINTER method)
            {
                const job::type J(&host,method);
                return yield(J);
            }

        protected:
            explicit  supervisor() throw();
            job::uuid jid; //!< shared job id to build uuids

        private:
            Y_DISABLE_COPY_AND_ASSIGN(supervisor);
        };

    }
}


#endif

