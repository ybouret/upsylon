
//! \file
#ifndef Y_RTLD_INCLUDED
#define Y_RTLD_INCLUDED 1

#include "y/string.hpp"
//#include "y/concurrent/singleton.hpp"

namespace upsylon
{
    //______________________________________________________________________
    //
    //
    //! run time loading
    //
    //______________________________________________________________________
    class rtld
    {
    public:
        //______________________________________________________________________
        //
        //
        // system API
        //
        //______________________________________________________________________
        static void  *init(const string &soname);   //!< open a shared object
        static void  *init(const char   *soname);   //!< open a shared object
        static void   quit(void * &handle) throw(); //!< free a shared object
        static void  *load(const void *handle, const string &symbol) throw(); //!< find symbol address
        static void  *load(const void *handle, const char   *symbol) throw(); //!< find symbol address


    private:
        Y_DISABLE_COPY_AND_ASSIGN(rtld);
    };
}

#endif

