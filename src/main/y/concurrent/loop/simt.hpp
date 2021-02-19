
//! \file

#ifndef Y_CONCURRENT_LOOP_SIMT_INCLUDED
#define Y_CONCURRENT_LOOP_SIMT_INCLUDED 1

#include "y/concurrent/thread.hpp"
#include "y/concurrent/executable.hpp"
#include "y/concurrent/sync/condition.hpp"
#include "y/sequence/slots.hpp"
#include "y/ptr/auto.hpp"

//! VERBOSITY
#define Y_VERBOSE_SIMT "Y_VERBOSE_SIMT"

//! macro helper
#define Y_SIMT_LN(MSG) do { if(verbose) { std::cerr << MSG << std::endl; } } while(false)

namespace upsylon
{

    namespace concurrent
    {
        //!interface to runnable object
        class runnable
        {
        public:
            virtual ~runnable() throw() {}
            explicit runnable() throw()
            {
            }
            
            virtual void run(const context &, lockable &) = 0;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(runnable);
        };



        class simt : public executable
        {
        public:
            explicit simt();
            virtual ~simt() throw();

            void loop(runnable &);
            void join() throw();

            const size_t cycles;
        private:
            Y_DISABLE_COPY_AND_ASSIGN(simt);
            runnable       *code;
            size_t          ready;
            condition       cycle;
            size_t          joined;
            condition       finish;
            slots<launcher> crew;
            bool            built;
            
            void         setup();
            void         cleanup() throw();
            virtual void call(const context &) throw();
        public:
            bool verbose;
        };

    }

}

#endif
