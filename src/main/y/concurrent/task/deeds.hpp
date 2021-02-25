//! \file

#ifndef Y_CONCURRENT_TASK_DEEDS_INCLUDED
#define Y_CONCURRENT_TASK_DEEDS_INCLUDED 1

#include "y/concurrent/task/deed.hpp"

namespace upsylon
{
    namespace concurrent
    {
        
        class deeds : public core::list_of<deed>, public releasable
        {
        public:
            virtual ~deeds() throw();
            
        protected:
            explicit deeds() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(deeds);
            
        };
        
        class pending_deeds : public deeds
        {
        public:
            explicit     pending_deeds() throw();
            virtual     ~pending_deeds() throw();
            virtual void release()       throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(pending_deeds);
        };
        
        class shallow_deeds : public deeds
        {
        public:
            explicit shallow_deeds() throw();
            virtual ~shallow_deeds() throw();
            virtual void release() throw();
            void         reserve(size_t n);
            void         cancel(deed *)          throw();
            void         cancel(pending_deeds &) throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(shallow_deeds);
        };
        
     
        
       
        
    }
    
}

#endif
