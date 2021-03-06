#include "y/lockable.hpp"

namespace upsylon
{
    lockable:: ~lockable() throw() {}
    lockable::  lockable() throw() {}

    scoped_lock:: ~scoped_lock() throw()                               { host_.unlock(); }
    scoped_lock::  scoped_lock( lockable &host ) throw() : host_(host) { host_.lock();   }

}
