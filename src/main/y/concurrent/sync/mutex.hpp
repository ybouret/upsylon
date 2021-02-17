//! \file
#ifndef Y_CONCURRENT_MUTEX_INCLUDED
#define Y_CONCURRENT_MUTEX_INCLUDED 1

#include "y/concurrent/nucleus/mutex.hpp"

namespace upsylon
{
 
    namespace concurrent
    {

        class condition; //!< forward declaration

        //! system recursive mutex
        class mutex : public lockable
        {
        public:

            explicit     mutex()    throw(); //!< create the system mutex
            virtual     ~mutex()    throw(); //!< release all
            virtual void lock()     throw(); //!< lock mutex
            virtual void unlock()   throw(); //!< unlock mutex
            virtual bool try_lock() throw(); //!< try lock mutex
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(mutex);
            nucleus::mutex::type m;
            friend class condition;

        public:
            static mutex giant; //!< a giant mutex for global locking
        };

        //! put a scoped lock on the giant mutex
#define Y_GIANT_LOCK() Y_LOCK(concurrent::mutex::giant)

        //! probing mutex until valid condition is reached
#define Y_MUTEX_PROBE(M,VALID_CONDITION) do {     \
if( !(M).try_lock()   ) continue;                 \
Y_LOCK(M);                                        \
if( (VALID_CONDITION) ) { (M).unlock(); break; }  \
(M).unlock();                                     \
} while(true)

    }
    
}


#endif

