//! \file 
#ifndef Y_LOCKABLE_INCLUDED
#define Y_LOCKABLE_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    //! lockable interface
    class lockable
    {
    public:
        //! default destructor
        inline virtual ~lockable() throw() {}
        
        //! must lock, critical error otherwise
        virtual void lock()     throw() = 0;
        
        //! must unlock, critical error otherwise
        virtual void unlock()   throw() = 0;
        
        //! must try to lock, critical error otherwise
        virtual bool try_lock() throw() = 0;
        
    protected:
        //! default constructor, do nothing
        inline explicit lockable() throw() {}
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(lockable);
    };
    
    //! local locking mechanism
    class scoped_lock
    {
    public:
        //! lock host
        inline explicit scoped_lock( lockable &host ) throw() : host_(host)
        {  host_.lock(); }
        
        //! unlock host
        inline ~scoped_lock() throw() { host_.unlock(); }
        
    private:
        lockable &host_;
        Y_DISABLE_COPY_AND_ASSIGN(scoped_lock);
    };
    
    //! create the guard name
#define Y_LOCK__(X,Y) X##Y
    
    //! instantiace the guard name
#define Y_LOCK_(HOST,ID) upsylon::scoped_lock  Y_LOCK__(__guard,ID)(HOST)
    
    //! use a local scoped_lock to lock HOST
#define Y_LOCK(HOST) Y_LOCK_(HOST,__LINE__)
    
}

#endif

