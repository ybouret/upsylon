
//! \file
#ifndef Y_IOS_ENDL_INCLUDED
#define Y_IOS_ENDL_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    namespace ios
    {

        //! implementing a self contained endl type
#define Y_IOS_DECL_ENDL(CLASS,BYTES)                               \
/**/    class CLASS##_t {                                          \
/**/      public:                                                  \
/**/        static const char   expr[];                            \
/**/        static const size_t size = BYTES;                      \
/**/        inline  CLASS##_t() throw() {}                         \
/**/        inline ~CLASS##_t() throw() {}                         \
/**/        inline  CLASS##_t(const CLASS##_t &) throw() {}        \
/**/        template <typename OSTREAM> inline friend              \
/**/        OSTREAM & operator<<(OSTREAM &os, const CLASS##_t &) { \
/**/            for(size_t i=0;i<size;++i) os << expr[i];          \
/**/            return os;     }                                   \
/**/      private:                                                 \
/**/        CLASS##_t & operator=(const CLASS##_t&);               \
/**/    };                                                         \
/**/    extern const CLASS##_t CLASS


        Y_IOS_DECL_ENDL(cr,1);        //!< CR
        Y_IOS_DECL_ENDL(lf,1);        //!< LF
        Y_IOS_DECL_ENDL(crlf,2);      //!< CRLF


    }

}

#endif
