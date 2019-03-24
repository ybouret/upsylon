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
            readable = 0x01, //!< readable bit
            writable = 0x02, //!< writable bit
            truncate = 0x80  //!< truncate on creation
        };

        //! for positioning
        enum whence_t
        {
            from_set, //!< from file ste
            from_cur, //!< form current position
            from_end  //!< from end of file
        };

        typedef int64_t offset_t; //!< alias

        //! low level local_file, built upon descriptor
        class local_file
        {
        public:
            typedef descriptor::type handle_t; //!< alias

            //! type of descriptor
            enum type_t
            {
                is_regular, //!< a local file system handle
                is_stdin,   //!< map to stdin
                is_stdout,  //!< map to stdout
                is_stderr,  //!< map to stderr
                is_pipe     //!< map to a named pipe
            };

            
            explicit local_file( const string &filename, const unsigned mode);  //!< open a file
            explicit local_file( const char   *filename, const unsigned mode);  //!< open a file
            virtual ~local_file() throw();                                      //!< desctructor
            local_file( const ios::cstdin_t  & );                               //!< map to stdin
            local_file( const ios::cstderr_t & );                               //!< map to stderr
            local_file( const ios::cstdout_t & );                               //!< map to stdout

            offset_t seek( offset_t delta, whence_t whence ); //!< move descriptor
            offset_t tell();   //!< current position
            void     rewind(); //!< go back to start of file
            void     unwind(); //!< go to end of file
            size_t   length(); //!< return length

            //! helper 
            inline bool is_readable() const throw() { return 0 != ( access & readable); }
            //! helper
            inline bool is_writable() const throw() { return 0 != ( access & writable); }

            //! access operator, copy of handle
            descriptor::type operator*() throw();
            
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

