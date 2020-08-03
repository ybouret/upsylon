//! \file
#ifndef Y_ERROR_INCLUDED
#define Y_ERROR_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    //! system error type
#if defined(Y_BSD)
    typedef int      error_type;
#endif

#if defined(Y_WIN)
    typedef uint32_t error_type;
#endif

    namespace libc
    {
        //! format libc error into buffer
        void format_error( char buffer[], size_t length, int err ) throw();
        
        //! abort execution with context 'when'
        void critical_error( int err, const char when[] ) throw();
    }

    namespace win32
    {
        //! format win32 error into buffer
        void format_error( char buffer[], size_t length, uint32_t err ) throw();
        
        //! abort execution with context 'when'
        void critical_error( uint32_t err, const char when[] ) throw();
    }

    //! default system error format into buffer
    void format_error( char buffer[], size_t length, error_type err ) throw();
    
    //! abort program with system error and context 'when'
    void critical_error( error_type err, const char when[] ) throw();
    
    //! an opaque invalid_data error
    extern const error_type error_invalid_data;
    
    //! a generic program abort upon system failure
    inline void fatal_error(const char when[]) throw()
    {
        critical_error(error_invalid_data,when);
    }

}

#endif
