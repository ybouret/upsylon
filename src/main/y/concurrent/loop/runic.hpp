

//! \file

#ifndef Y_CONCURRENT_LOOP_RUNIC_INCLUDED
#define Y_CONCURRENT_LOOP_RUNIC_INCLUDED 1

#include "y/concurrent/loop/types.hpp"
#include "y/memory/shacks.hpp"


namespace upsylon
{
    
    namespace concurrent
    {
        
        
        class runic :
        public ref_ptr<looper>,
        public memory::shacks,
        public runnable
        {
        public:
            typedef memory::shack  cache;
            
            explicit runic( const arc_ptr<looper> & );
            explicit runic( looper * );

         
            
            virtual ~runic() throw();
            
            typedef void (*proc)(const context &,
                                 memory::shack &,
                                 void          *,
                                 lockable      &);
            
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

