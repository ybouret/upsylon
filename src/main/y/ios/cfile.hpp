//! \file
#ifndef Y_IOS_CFILE_INCLUDED
#define Y_IOS_CFILE_INCLUDED 1

#include "y/string.hpp"
#include <cstdio>

namespace upsylon
{
    namespace ios
    {

        typedef int2type<0> cstdin_t;  extern const cstdin_t  cstdin;
        typedef int2type<1> cstdout_t; extern const cstdout_t cstdout;
        typedef int2type<2> cstderr_t; extern const cstderr_t cstderr;


        //! wrapper for FILE *
        class cfile : public virtual object
        {
        public:
            enum open_mode
            {
                open_read,
                open_write,
                open_append
            };

            //! close
            virtual ~cfile() throw();

            //! open filename with mode
            cfile( const string &filename, const open_mode m);
            //! open filename with mode
            cfile( const char   *filename, const open_mode m);
            //! open stdin
            cfile( const cstdin_t  & );
            cfile( const cstderr_t & );
            cfile( const cstdout_t & );

            //! content operator
            FILE * operator*() throw();



        protected:
            FILE       *fp;
            const bool  persistent;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(cfile);
        };
    }
}
#endif
