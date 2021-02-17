
//! \file

#ifndef Y_CONCURRENT_LOOP_SIMT_INCLUDED
#define Y_CONCURRENT_LOOP_SIMT_INCLUDED 1

#include "y/concurrent/thread.hpp"
#include "y/concurrent/topology.hpp"
#include "y/sequence/slots.hpp"
#include "y/ptr/auto.hpp"

#define Y_VERBOSE_SIMT "Y_VERBOSE_SIMT"

namespace upsylon
{

    namespace concurrent
    {

        class simt
        {
        public:
            explicit simt();
            virtual ~simt() throw();

            const auto_ptr<const topology> topo;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(simt);
            slots<thread>  crew;

            void setup();
            void cleanup() throw();

            static inline void stub(void *self) throw();
            
        public:
            bool verbose;
        };

    }

}

#endif
