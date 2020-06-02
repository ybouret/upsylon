#include "y/fs/find.hpp"
#include "y/sequence/pipe.hpp"

namespace upsylon {

    fs_find:: directories::  directories() throw() {}
    fs_find:: directories:: ~directories() throw() {}
    fs_find:: look_up::     ~look_up()     throw() {}

    bool fs_find::accept_any(const vfs::entry&) throw()
    {
        return true;
    }

   

    
}
