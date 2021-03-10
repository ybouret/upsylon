
//! \file

#ifndef Y_CONCURRENT_LOOP_SIMT_INCLUDED
#define Y_CONCURRENT_LOOP_SIMT_INCLUDED 1

#include "y/concurrent/loop/types.hpp"
#include "y/concurrent/thread.hpp"
#include "y/concurrent/component.hpp"
#include "y/concurrent/sync/condition.hpp"
#include "y/sequence/slots.hpp"
#include "y/type/authority.hpp"

//! macro helper
#define Y_SIMT_LN(MSG) do { if(verbose) { std::cerr << MSG << std::endl; } } while(false)

namespace upsylon
{

    namespace concurrent
    {
        //______________________________________________________________________
        //
        //
        //! Single Instruction, Multiple Threads
        //
        //______________________________________________________________________
        class simt : public component, public looper
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit simt();                       //!< automatic setup
            explicit simt(Y_CONCURRENT_TOPO_ARGS); //!< manual setup
            virtual ~simt() throw();  //!< cleanup

            //__________________________________________________________________
            //
            // looper interface
            //__________________________________________________________________
            virtual void        for_each(runnable &) throw();
            virtual const char *category()     const throw();

            //__________________________________________________________________
            //
            // accessible<context> interface
            //__________________________________________________________________
            virtual size_t         size()                   const throw(); //!< num threads
            virtual const context &operator[](const size_t) const throw(); //!< in 1..num_threads
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(simt);
            class launcher : public authority<simt>, public thread
            {
            public:
                virtual ~launcher() throw();
                explicit launcher(simt        &user_host,
                                  const size_t user_size,
                                  const size_t user_rank);
                
            private:
                static void stub(void*) throw();
                Y_DISABLE_COPY_AND_ASSIGN(launcher);
            };


            runnable       *code;
            size_t          ready;
            condition       cycle;
            condition       fence;
            slots<launcher> crew;
            bool            built;
            
            void setup();
            void cleanup() throw();
            void call(const context &) throw(); //!< threaded function

        public:
            bool verbose; //!< from Y_VERBOSE_THREADS env
        };

    }

}

#endif
