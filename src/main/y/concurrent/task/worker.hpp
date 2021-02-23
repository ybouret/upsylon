
//! \file

#ifndef Y_CONCURRENT_TASK_WORKER_INCLUDED
#define Y_CONCURRENT_TASK_WORKER_INCLUDED 1

#include "y/concurrent/task/types.hpp"
#include "y/concurrent/executable.hpp"
#include "y/concurrent/sync/condition.hpp"
#include "y/core/inode.hpp"

namespace upsylon
{
    namespace concurrent
    {

        class worker;
        typedef core::list_of_cpp<worker> workers;

        class team : public executable
        {
        public:
            class reference
            {
            public:
                virtual ~reference() throw() {}
                explicit reference(team &t) throw() : crew(t) {}

            protected:
                team &crew;
            };

            virtual ~team() throw();
            explicit team();

        private:
            workers waiting;
            size_t  ready;
            Y_DISABLE_COPY_AND_ASSIGN(team);
            virtual void call(const context &);
            void         setup();
            friend class worker;
        };

        class worker :
        public object,
        public condition,
        public team::reference,
        public thread,
        public inode<worker>
        {
        public:
            explicit worker(team        &user_team,
                            const size_t user_size,
                            const size_t user_rank);

            virtual ~worker() throw();

            
        private:
            static void stub(void*) throw();
            void        initialize() throw();
        };


    }

}

#endif
