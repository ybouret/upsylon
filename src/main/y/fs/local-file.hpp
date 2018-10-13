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

            
            virtual ~local_file() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(local_file);

        protected:
            handle_t       handle;   //!< the file descriptor
        public:
            const unsigned access;   //!< readable/writable
            const type_t   type;     //!< core type
            error_type    *status;   //!< to store status is not NULL

        };

    }
}

#endif

