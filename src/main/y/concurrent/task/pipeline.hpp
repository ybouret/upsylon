
//! \file

#ifndef Y_CONCURRENT_TASK_PIPELINE_INCLUDED
#define Y_CONCURRENT_TASK_PIPELINE_INCLUDED 1

#include "y/concurrent/task/type.hpp"
#include "y/concurrent/executable.hpp"
#include "y/concurrent/thread.hpp"
#include "y/core/inode.hpp"

//! VERBOSITY
#define Y_VERBOSE_PIPELINE "Y_VERBOSE_PIPELINE"

//! macro helper
#define Y_PIPELINE_LN(MSG) do { if(verbose) { std::cerr << MSG << std::endl; } } while(false)


namespace upsylon
{
    namespace concurrent
    {


        class pipeline : public executable
        {
        public:
            class engine : public object, public launcher, public inode<engine>
            {
            public:
                explicit engine(const pipeline &srv,
                                const size_t    user_size,
                                const size_t    user_rank);
                virtual ~engine() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(engine);
            };

            typedef core::list_of_cpp<engine> engines;

            virtual ~pipeline() throw();
            explicit pipeline();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pipeline);
            virtual void call(const context &) throw();

            engines waiting;
            engines working;

            void setup();

        public:
            bool verbose;
            
        };
    }
}


#endif

