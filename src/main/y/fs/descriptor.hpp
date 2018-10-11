//! \file
#ifndef Y_FS_DESCRIPTOR_INCLUDED
#define Y_FS_DESCRIPTOR_INCLUDED 1

#include "y/os/error.hpp"

#if defined(Y_BSD)
#define Y_FD int
#endif

#if defined(Y_WIN)
#define Y_FD void *
#endif

namespace upsylon
{
    namespace ios
    {
        //! file descriptor basics
        struct descriptor
        {
            typedef Y_FD      type;              //!< alias
            static type       invalid() throw(); //!< invalid for OS
            static void       get( type handle, void       *data, size_t size, size_t &done ); //!< get data
            static void       put( type handle, const void *data, size_t size, size_t &done ); //!< put data
            static error_type close(type handle) throw(); //!< close descriptor
        };
    }
}

#endif

