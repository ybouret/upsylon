#include "y/concurrent/mutex.hpp"

namespace upsylon
{
    namespace concurrent
    {
        mutex::  mutex() throw() : m()  { nucleus::mutex::init(&m); }
        mutex:: ~mutex() throw()        { nucleus::mutex::quit(&m); }

        void mutex:: lock()   throw() { nucleus::mutex::lock(&m);   }
        void mutex:: unlock() throw() { nucleus::mutex::unlock(&m); }
        bool mutex:: try_lock() throw() { return nucleus::mutex::try_lock(&m); }

        mutex mutex::giant;
        

    }

}

