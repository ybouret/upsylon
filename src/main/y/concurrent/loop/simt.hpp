
//! \file

#ifndef Y_CONCURRENT_LOOP_SIMT_INCLUDED
#define Y_CONCURRENT_LOOP_SIMT_INCLUDED 1

#include "y/concurrent/loop/types.hpp"
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
        class simt : public executable, public looper
        {
        public:
            explicit simt();
            virtual ~simt() throw();

            virtual void for_each(runnable &) throw();

            virtual size_t         size()                   const throw();
            virtual const context &operator[](const size_t) const throw();
            

        private:
            Y_DISABLE_COPY_AND_ASSIGN(simt);
            runnable       *code;
            size_t          ready;
            condition       cycle;
            condition       fence;
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
