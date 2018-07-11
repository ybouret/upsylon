//! \file
#ifndef Y_IOS_CFILE_INCLUDED
#define Y_IOS_CFILE_INCLUDED 1

#include "y/string.hpp"
#include <cstdio>

namespace upsylon
{
    namespace ios
    {

        typedef int2type<0>    cstdin_t;  //!< named type for stdin
        typedef int2type<1>    cstdout_t; //!< named type for stdout
        typedef int2type<2>    cstderr_t; //!< named type for stderr

        extern const cstdin_t  cstdin;   //!< for type selection
        extern const cstdout_t cstdout;  //!< for type selection
        extern const cstderr_t cstderr;  //!< for type selection

        //! wrapper for FILE *
        class cfile : public virtual object
        {
        public:
            //! how to open it
            enum open_mode
            {
                open_read,    //!< "rb"
                open_write,   //!< "wb"
                open_append   //!< "ab"
            };

            //! close
            virtual ~cfile() throw();

            //! open filename with mode
            cfile( const string &filename, const open_mode m);
            //! open filename with mode
            cfile( const char   *filename, const open_mode m);
            //! open stdin
            cfile( const cstdin_t  & );
            //! open stderr
            cfile( const cstderr_t & );
            //! open stdout
            cfile( const cstdout_t & );

            //! content operator
            FILE * operator*() throw();

        private:
            FILE       *fp;
            const bool  persistent;
            Y_DISABLE_COPY_AND_ASSIGN(cfile);
        };
    }
}
#endif
