
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
        //______________________________________________________________________
        //
        //
        //! Single Instruction, Multiple Threads
        //
        //______________________________________________________________________
        class simt : public executable, public looper
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit simt();          //!< setup
            virtual ~simt() throw();  //!< cleanup

            //__________________________________________________________________
            //
            // looper interface
            //__________________________________________________________________
            virtual void for_each(runnable &) throw();

            //__________________________________________________________________
            //
            // accessible<context> interface
            //__________________________________________________________________
            virtual size_t         size()                   const throw(); //!< num threads
            virtual const context &operator[](const size_t) const throw(); //!< in 1..num_threads
            
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
            bool verbose; //!< from Y_VERBOSE_SIMT env
        };

    }

}

#endif
