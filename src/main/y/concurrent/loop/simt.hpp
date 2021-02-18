
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
        
        class simt
        {
        public:
            explicit simt();
            virtual ~simt() throw();
            
            mutex                          access;
            const auto_ptr<const topology> topo;
            
            void loop(runnable &);
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(simt);
            class launcher : public thread
            {
            public:
                explicit launcher(simt        &user_host,
                                  const size_t user_size,
                                  const size_t user_rank);
                virtual ~launcher() throw();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(launcher);
                simt &host;
                static void stub(void *);
            };
            
            runnable       *code;
            size_t          ready;
            condition       cycle;
            size_t          joined;
            condition       finish;
            slots<launcher> crew;
            bool            built;
            
            void setup();
            void cleanup() throw();
            void run(const context &) throw();
 
        public:
            bool verbose;
        };

    }

}

#endif
