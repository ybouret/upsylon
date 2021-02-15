
//! \file

#ifndef Y_CONCURRENT_CREW_INCLUDED
#define Y_CONCURRENT_CREW_INCLUDED 1


#include "y/concurrent/device/topology.hpp"
#include "y/concurrent/worker.hpp"
#include "y/concurrent/sync/condition.hpp"

#include "y/ptr/auto.hpp"
#include "y/sequence/slots.hpp"

#define Y_VERBOSE_THREADS "Y_VERBOSE_THREADS"

namespace upsylon
{
    namespace concurrent
    {
        namespace nucleus
        {
            class crew
            {
            public:
                virtual ~crew() throw();
                
                const auto_ptr<const topology> topo;
                
            protected:
                explicit crew();
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(crew);
            };
        }
        
#define Y_CREW_PRINTLN(MSG) do { if(verbose) { std::cerr << MSG << std::endl; } } while(false)
        
        //! low level crew of workers
        class crew : public nucleus::crew
        {
        public:
            enum status
            {
                anchored,
                launched
            };
            explicit crew();
            virtual ~crew() throw();
            
            mutex synchronize;
            void  run(executable code, void *data);
            
            static bool query_threads_verbosity(); //!< from Y_VERBOSE_THREADS
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(crew);
            slots<worker> squad;
            size_t        ready;
            condition     start;
            condition     stall;
            status        state;
            executable    kcode;
            void         *kdata;
            
            void        initialize();               //!< setup all waiting
            static void entry_stub(void *) throw(); //!< call entry()
            void        entry() throw();            //!< threaded function
            
        public:
            bool  verbose;
            
        };
    }
    
}

#endif

