//! \file

#ifndef Y_CONCURRENT_LOOP_RUNIC_INCLUDED
#define Y_CONCURRENT_LOOP_RUNIC_INCLUDED 1

#include "y/concurrent/loop/types.hpp"
#include "y/memory/shacks.hpp"


namespace upsylon
{
    
    namespace concurrent
    {
        
        //______________________________________________________________________
        //
        //
        //! Runnning In C
        /**
         local cache are handled and passed to C functions
         */
        //
        //______________________________________________________________________
        class runic :
        public ref_ptr<looper>,
        public memory::shacks,
        public runnable
        {
        public:
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            
            //! C-style procedure, call for each context with private memory
            typedef void (*proc)(const context &,
                                 memory::shack &,
                                 void          *,
                                 lockable      &);
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit runic(const arc_ptr<looper> &); //!< setup from a shared looper
            explicit runic(looper * );               //!< setup from a new looper
            virtual ~runic() throw();                //!< cleanup
            
            
           
            //__________________________________________________________________
            //
            //! calling a C procedure for each context, providing local memory
            //__________________________________________________________________
            void operator()(proc,void*) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(runic);
            proc  kProc;
            void *kArgs;
            
            virtual void run(const context &, lockable &) throw();
            
        };
        
        
        
    }
}

#endif

