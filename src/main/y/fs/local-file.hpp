//! \file
#ifndef Y_FS_LOCAL_FILE_INCLUDED
#define Y_FS_LOCAL_FILE_INCLUDED 1

#include "y/ios/cfile.hpp"
#include "y/fs/descriptor.hpp"

namespace upsylon
{
    namespace ios
    {
        enum
        {
            readable = 0x01,
            writable = 0x02,
            truncate = 0x80
        };

        enum whence_t
        {
            from_set,
            from_cur,
            from_end
        };

        typedef int64_t offset_t;

        class local_file
        {
        public:
            typedef descriptor::type handle_t;

            enum type_t
            {
                is_regular,
                is_stdin,
                is_stdout,
                is_stderr,
                is_pipe
            };

            explicit local_file( const string &filename, const unsigned mode);
            explicit local_file( const char   *filename, const unsigned mode);
            virtual ~local_file() throw();
            local_file( const ios::cstdin_t  & );
            local_file( const ios::cstderr_t & );
            local_file( const ios::cstdout_t & );

            offset_t seek( offset_t delta, whence_t whence );
            offset_t tell();
            void     rewind();
            void     unwind();
            size_t   length();

            inline bool is_readable() const throw() { return 0 != ( access & readable); }
            inline bool is_writable() const throw() { return 0 != ( access & writable); }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(local_file);

        protected:
            local_file( handle_t user_handle, size_t user_access ) throw(); //!< for pipe creation..
            handle_t       handle;   //!< the file descriptor
        public:
            const unsigned access;   //!< readable/writable
            const type_t   type;     //!< core type
            error_type    *status;   //!< to store status is not NULL

        };

    }
}

#endif

