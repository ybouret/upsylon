
//! \file

#ifndef Y_CONCURRENT_LOOP_SIMT_INCLUDED
#define Y_CONCURRENT_LOOP_SIMT_INCLUDED 1

#include "y/concurrent/thread.hpp"
#include "y/concurrent/topology.hpp"
#include "y/concurrent/sync/condition.hpp"
#include "y/sequence/slots.hpp"
#include "y/ptr/auto.hpp"

#define Y_VERBOSE_SIMT "Y_VERBOSE_SIMT"
#define Y_SIMT_LN(MSG) do { if(verbose) { std::cerr << MSG << std::endl; } } while(false)

namespace upsylon
{

    namespace concurrent
    {
        class simt
        {
        public:
            explicit simt();
            virtual ~simt() throw();
            
            mutex                          access;
            const auto_ptr<const topology> topo;
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(simt);
            typedef runnable<simt>         thread_type;
            
            size_t                         ready;
            condition                      cycle;
            size_t                         joined;
            condition                      finish;
            slots<thread_type>             crew;
            bool                           built;
            
            void setup();
            void cleanup() throw();
            void run(const context &) throw();
            friend thread_type;
            
        public:
            bool verbose;
        };

    }

}

#endif
